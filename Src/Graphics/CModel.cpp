#include "CModel.h"
#include "CColor4.h"

bool CModel::Init(
	const std::string& aFilePath,
	ID3D11Device* aDevice,
	ID3D11DeviceContext* aDeviceContext,
	CConstantBuffer<CBVertexShader>& aCBVertexShader)
{
	mDevice = aDevice;
	mDeviceContext = aDeviceContext;
	mCBVertexShader = &aCBVertexShader;

	try
	{
		if (!LoadModel(aFilePath)) return false;
	}
	catch (CComException& exception)
	{
		CErrorLogger::Log(exception);
		return false;
	}
}

void CModel::Render(const XMMATRIX& aModelViewProjectionMatrix) const
{
	mCBVertexShader->mData.Transform = XMMatrixTranspose(aModelViewProjectionMatrix);
	mCBVertexShader->Update();

	mDeviceContext->VSSetConstantBuffers(0, 1, mCBVertexShader->GetAddressOf());

	for (const auto& Mesh : mMeshes) Mesh.Render();

}

bool CModel::LoadModel(const std::string& aFilePath)
{
	mDirectory = CStringHelper::GetDirectory(aFilePath);

	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(aFilePath,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!pScene) return false;

	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void CModel::ProcessNode(aiNode* aNode, const aiScene* aScene)
{
	for (UINT i = 0; i < aNode->mNumMeshes; i++) {
		aiMesh* Mesh = aScene->mMeshes[aNode->mMeshes[i]];
		mMeshes.push_back(ProcessMesh(Mesh, aScene));
	}

	for (UINT i = 0; i < aNode->mNumChildren; i++) {
		ProcessNode(aNode->mChildren[i], aScene);
	}
}

CMesh CModel::ProcessMesh(aiMesh* aMesh, const aiScene* aScene)
{
	std::vector<TVertex> Vertices;
	std::vector<DWORD> Indices;

	for (UINT i = 0; i < aMesh->mNumVertices; i++) {
		const auto& vtx = aMesh->mVertices[i];
		TVertex Vertex(vtx.x, vtx.y, vtx.z, 0.f, 0.f);

		if (aMesh->mTextureCoords[0])
		{
			const auto& uv = aMesh->mTextureCoords[0][i];
			Vertex.TexCoord = { uv.x, uv.y };
		}

		Vertices.push_back(Vertex);
	}

	for (UINT i = 0; i < aMesh->mNumFaces; i++) {
		const aiFace& face = aMesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++) {
			Indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<CTexture> Textures;
	auto Material = aScene->mMaterials[aMesh->mMaterialIndex];
	std::vector<CTexture> DiffuseTextures = LoadMaterialTextures(Material, aiTextureType::aiTextureType_DIFFUSE, aScene);
	Textures.insert(Textures.end(), std::make_move_iterator(DiffuseTextures.begin()), std::make_move_iterator(DiffuseTextures.end()));
	DiffuseTextures.erase(DiffuseTextures.begin(), DiffuseTextures.end());

	return CMesh(mDevice, mDeviceContext, Vertices, Indices, Textures);
}

std::vector<CTexture> CModel::LoadMaterialTextures(const aiMaterial* aMaterial, aiTextureType aType, const aiScene* aScene) const
{
	std::vector<CTexture> MaterialTextures;
	unsigned int TextureCount = aMaterial->GetTextureCount(aType);

	if (TextureCount == 0) {
		CColor4 Color = GetMaterialColor(aMaterial, AI_MATKEY_COLOR_DIFFUSE);
		MaterialTextures.emplace_back(mDevice, aType, Color);
	}
	else {
		for (UINT i = 0; i < TextureCount; i++) {
			aiString Path;
			aMaterial->GetTexture(aType, i, &Path);
			ETextureStorageType StoreType = DetermineTextureStorageType(aScene, aMaterial, i, aType);

			switch (StoreType) {
			case ETextureStorageType::EmbeddedIndexCompressed:
			{
				const aiTexture* pTexture = GetEmbeddedIndexTexture(aScene, Path); 
				MaterialTextures.emplace_back(mDevice, aType, pTexture->pcData, pTexture->mWidth);
			}
			case ETextureStorageType::EmbeddedCompressed:
			{
				const aiTexture* pTexture = aScene->GetEmbeddedTexture(Path.C_Str());
				MaterialTextures.emplace_back(mDevice, aType, pTexture->pcData, pTexture->mWidth);
			}
			case ETextureStorageType::Disk:
			{
				std::string filename = mDirectory + '/' + Path.C_Str();
				MaterialTextures.emplace_back(mDevice, aType, filename);
				break;
			}
			}
		}
		MaterialTextures.emplace_back(mDevice, aType, color::UnhandledTextureColor);
	}

	if (MaterialTextures.size() == 0) MaterialTextures.emplace_back(mDevice, aType, color::UnhandledTextureColor);
	return MaterialTextures;
}

ETextureStorageType CModel::DetermineTextureStorageType(const aiScene* aScene, const aiMaterial* aMaterial, unsigned int aIndex, aiTextureType aType) const {
	if (aMaterial->GetTextureCount(aType) == 0) return ETextureStorageType::None;

	ETextureStorageType Result;
	
	std::string Path = GetTexturePath(aMaterial, aType, aIndex);
	if (Path.front() == '*') {
		if (aScene->mTextures[0]->mHeight == 0) {
			Result = ETextureStorageType::EmbeddedIndexCompressed;
		}
		else {
			CErrorLogger::Log("Support does not exist yet for indexed non compressed textures");
			Result = ETextureStorageType::EmbeddedIndexNonCompressed;
		}
	}
	else if (auto Texture = aScene->GetEmbeddedTexture(Path.c_str())) {
		if (Texture->mHeight == 0) {
			Result = ETextureStorageType::EmbeddedCompressed;
		}
		else {
			CErrorLogger::Log("Support does not exist yet for Embedded non compressed textures");
			Result = ETextureStorageType::EmbeddedNonCompressed;
		}
	}
	else if (Path.find('.') != std::string::npos) {
		Result = ETextureStorageType::Disk;
	}
	else
	{
		Result = ETextureStorageType::None;
	}
	return Result;
}

CColor4 CModel::GetMaterialColor(const aiMaterial* aMaterial, const char* aKey, unsigned int aType, unsigned int aIndex) const {
	aiColor4D Color;
	aMaterial->Get(aKey, aType, aIndex, Color);
	if (Color.IsBlack()) return color::UnloadedTextureColor;
	return CColor4(Color);
}

std::string CModel::GetTexturePath(const aiMaterial* aMaterial, aiTextureType aType, unsigned int aIndex) const {
	aiString aiPath;
	aMaterial->GetTexture(aType, aIndex, &aiPath);
	return aiPath.C_Str();
}

const aiTexture* CModel::GetEmbeddedIndexTexture(const aiScene* aScene, const aiString& aPath) const {
	assert(aPath.length >= 2);
	int index = atoi(&aPath.C_Str()[1]);
	return aScene->mTextures[index];
}
