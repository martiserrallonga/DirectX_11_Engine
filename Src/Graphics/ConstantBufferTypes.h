#pragma once
#include <DirectXMath.h>

struct CBVertexShader
{
	DirectX::XMMATRIX Transform;
};

struct CBPixelShader
{
	float Alpha;
};