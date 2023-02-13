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

struct CBLight
{
	DirectX::XMFLOAT3 AmbientColor;
	float AmbientStrength;

	DirectX::XMFLOAT3 DirectionalColor;
	float DirectionalStrength;
	
	DirectX::XMFLOAT3 DirectionalPosition;
};