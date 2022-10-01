#pragma once

#include "CTransform.h"
#include "CMesh.h"
#include "CConstantBuffer.h"

class CModel : public CTransform
{
public:
	bool Init(
		const std::string &aFilePath
		, ID3D11Device* aDevice
		, ID3D11DeviceContext* aDeviceContext
		, ID3D11ShaderResourceView* aTexture
		, CConstantBuffer<CBVertexShader>& aCBVertexShader
	);

	void Render(const XMMATRIX& aViewProjectionMatrix);
	void SetTexture(ID3D11ShaderResourceView* aTexture);

	void UpdateMatrix() override { UpdateWorldMatrix(); }

private:
	bool LoadModel(const std::string& aFilePath);
	void ProcessNode(aiNode* aNode, const aiScene* aScene);
	CMesh ProcessMesh(aiMesh* aMesh, const aiScene* aScene);

	std::vector<CMesh> mMeshes;

	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;
	CConstantBuffer<CBVertexShader>* mCBVertexShader = nullptr;
	ID3D11ShaderResourceView* mTexture = nullptr;

	CVertexBuffer<TVertex> mVertexBuffer;
	CIndexBuffer mIndexBuffer;

};