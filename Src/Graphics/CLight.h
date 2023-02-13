#pragma once
#include "CGameEntity.h"

class CLight
{
public: 
	void Init(CTransform* aTransform, CConstantBuffer<CBLight>* aConstBuffer);

	void Update() const;

	void RenderImGui();

	DirectX::XMFLOAT3 mAmbientColor;
	float mAmbientStrength;

	DirectX::XMFLOAT3 mDirectionalColor;
	float mDirectionalStrength;

	DirectX::XMFLOAT3 mDirectionalPosition;

private:
	CTransform* mpTransform;
	CConstantBuffer<CBLight>* mpConstBuffer;

};

