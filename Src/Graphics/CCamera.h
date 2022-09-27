#pragma once
#include "CTransform.h"

class CCamera : public CTransform
{
public:
	void UpdateMatrix() override { UpdateViewMatrix(); }

	void SetProjectionValues(float aFov, float aAspect, float aNear, float aFar);

	const XMMATRIX& GetViewMatrix() const { return GetTransformMatrix(); }
	const XMMATRIX& GetProjectionMatrix() const { return mProjection; }


private:
	XMMATRIX mProjection;

};
