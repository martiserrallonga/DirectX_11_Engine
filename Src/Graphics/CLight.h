#pragma once
#include "CGameEntity.h"

class CLight
{
public: 
	void Init(CTransform* aTransform, CConstantBuffer<CBLight>* aConstBuffer);

	void Update() const;

	void RenderImGui();

	// Ambient
	DirectX::XMFLOAT3 mAmbientColor;
	float mAmbientStrength;

	// Directional
	DirectX::XMFLOAT3 mDirectionalColor;
	float mDirectionalStrength;
	DirectX::XMFLOAT3 mDirectionalPosition;

	// Attenuation
	float mAttenuation_A;
	float mAttenuation_B;
	float mAttenuation_C;

private:
	CTransform* mpTransform;
	CConstantBuffer<CBLight>* mpConstBuffer;

};

