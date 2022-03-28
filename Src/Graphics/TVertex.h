#pragma once
#include <DirectXMath.h>

struct TVertex
{
	TVertex() = default;
	TVertex(float x, float y)
		: Position{ x, y }
	{}

	DirectX::XMFLOAT2 Position;
};