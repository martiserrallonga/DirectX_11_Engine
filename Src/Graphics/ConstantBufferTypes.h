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

struct CBAmbientLight
{
	DirectX::XMFLOAT3 AmbientLight;
	float AmbientStrenght;
};