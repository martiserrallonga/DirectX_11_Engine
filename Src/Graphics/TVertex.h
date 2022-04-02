#pragma once
#include <DirectXMath.h>

struct TVertex
{
	TVertex() = default;
	TVertex(float x, float y, float z, float r, float g, float b)
		: Position(x, y, z)
		, Color(r, g, b)
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};