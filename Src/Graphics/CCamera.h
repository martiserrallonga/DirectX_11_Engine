#pragma once
#include "CTransform.h"

class CCamera : public CTransform
{
public:
	void SetProjectionValues(float aFov, float aAspect, float aNear, float aFar);
	const XMMATRIX& GetViewProjectionMatrix() const;



	float Speed = 3.f;

private:
	void UpdateMatrix() override;

	XMMATRIX mProjection;

};
