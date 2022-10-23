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
	
	aiMaterial* Material = aScene->mMaterials[aMesh->mMaterialIndex];
	std::vector<CTexture> DiffuseTextures = LoadMaterialTextures(Material, aiTextureType::aiTextureType_DIFFUSE, aScene);
	Textures.insert(Textures.end(), std::make_move_iterator(DiffuseTextures.begin()), std::make_move_iterator(DiffuseTextures.end()));
	DiffuseTextures.erase(DiffuseTextures.begin(), DiffuseTextures.end());

	return CMesh(mDevice, mDeviceContext, Vertices, Indices, Textures);
}

std::vector<CTexture> CModel::LoadMaterialTextures(aiMaterial* aMaterial, aiTextureType aTextureType, const aiScene* pScene)
{
	std::vector<CTexture> MaterialTextures;
	ETextureStorageType StoreType = ETextureStorageType::Invalid;
	unsigned int TextureCount = aMaterial->GetTextureCount(aTextureType);

	if (TextureCount == 0) {
		switch (aTextureType) {
		case aiTextureType_DIFFUSE:
			CColor4 Color = GetMaterialColor(aMaterial, AI_MATKEY_COLOR_DIFFUSE);
			if (Color.IsBlack()) {
				MaterialTextures.emplace_back(mDevice, aTextureType, color::UnloadedTextureColor);
			}
			else
			{
				MaterialTextures.emplace_back(mDevice, aTextureType, Color);
			}
		}
	}
	else
	{
		MaterialTextures.emplace_back(mDevice, aTextureType, color::UnhandledTextureColor);
	}

	return MaterialTextures;
}

CColor4 CModel::GetMaterialColor(aiMaterial* aMaterial, const char* aKey, unsigned int aType, unsigned int aIndex) const {
	aiColor4D Color;
	aMaterial->Get(aKey, aType, aIndex, Color);
	return CColor4(Color);
}