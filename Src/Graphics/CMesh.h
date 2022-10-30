#pragma once
#include "CTransform.h"
#include "TVertex.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CTexture.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

struct TVertex;

class CMesh : public CTransform
{
public:
	CMesh(ID3D11Device* aDevice,
		ID3D11DeviceContext* aDeviceContext,
		std::vector<TVertex> aVertices,
		std::vector<DWORD>& aIndices,
		std::vector<CTexture>& aTextures,
		const DirectX::XMMATRIX& aTransform);

	CMesh(const CMesh& aMesh) = default;

	void Render() const;

	void UpdateMatrix() override { UpdateWorldMatrix(); }

private:
	CVertexBuffer<TVertex> mVertexBuffer;
	CIndexBuffer mIndexBuffer;
	ID3D11DeviceContext* mDeviceContext;
	std::vector<CTexture> mTextures;

};

