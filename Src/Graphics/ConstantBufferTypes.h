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
	float dummy3;

	float Attenuation_A;
	float Attenuation_B;
	float Attenuation_C;
	float dummy4;
};