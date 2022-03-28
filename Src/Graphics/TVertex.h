#pragma once
#include <DirectXMath.h>

struct TVertex
{
	TVertex() = default;
	TVertex(float x, float y, float r, float g, float b)
		: Position(x, y)
		, Color(r, g, b)
	{}

	DirectX::XMFLOAT2 Position;
	DirectX::XMFLOAT3 Color;
};