#include "CModel.h"

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

	std::vector<CTexture> textures;
	//textures.emplace_back(mDevice, aiTextureType::aiTextureType_DIFFUSE, &TColor::UnloadedTextureColor);
	textures.emplace_back(mDevice, aiTextureType::aiTextureType_DIFFUSE, &TColor::UnloadedTextureColor);

	return CMesh(mDevice, mDeviceContext, Vertices, Indices, textures);
}
