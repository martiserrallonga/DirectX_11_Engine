#pragma once
#include <DirectXMath.h>

struct CBVertexShader
{
	DirectX::XMMATRIX MVP;
	DirectX::XMMATRIX World;
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