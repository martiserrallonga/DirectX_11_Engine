#pragma once
#include "TVertex.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

struct TVertex;

class CMesh
{
public:
	CMesh(ID3D11Device* aDevice
		, ID3D11DeviceContext* aDeviceContext
		, std::vector<TVertex> aVertices
		, std::vector<DWORD> &aIndices
	);

	CMesh(const CMesh& aMesh) = default;

	void Render() const;

private:
	CVertexBuffer<TVertex> mVertexBuffer;
	CIndexBuffer mIndexBuffer;
	ID3D11DeviceContext* mDeviceContext;

};

