#pragma once
#include <DirectXMath.h>

struct TVertex
{
	TVertex(float x, float y, float z, float u, float v)
		: Position(x, y, z)
		, TexCoord(u, v)
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};