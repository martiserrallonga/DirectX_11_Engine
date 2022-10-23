#pragma once

#include "CTransform.h"
#include "CMesh.h"
#include "CConstantBuffer.h"

class CModel
{
public:
	bool Init(
		const std::string& aFilePath,
		ID3D11Device* aDevice,
		ID3D11DeviceContext* aDeviceContext,
		CConstantBuffer<CBVertexShader>& aCBVertexShader);

	void Render(const XMMATRIX& aModelViewProjectionMatrix) const;

private:
	bool LoadModel(const std::string& aFilePath);
	void ProcessNode(aiNode* aNode, const aiScene* aScene);
	CMesh ProcessMesh(aiMesh* aMesh, const aiScene* aScene);

	std::vector<CTexture> LoadMaterialTextures(const aiMaterial* aMaterial, aiTextureType aType, const aiScene* pScene) const;
	ETextureStorageType DetermineTextureStorageType(const aiScene* aScene, const aiMaterial* aMaterial, unsigned int aIndex, aiTextureType aType) const;
	CColor4 GetMaterialColor(const aiMaterial* aMaterial, const char* aKey, unsigned int aType, unsigned int aIndex) const;
	std::string GetTexturePath(const aiMaterial* aMaterial, aiTextureType aType, unsigned int aIndex) const;


	std::vector<CMesh> mMeshes;
	std::string mDirectory;

	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;
	CConstantBuffer<CBVertexShader>* mCBVertexShader = nullptr;

	CVertexBuffer<TVertex> mVertexBuffer;
	CIndexBuffer mIndexBuffer;

};