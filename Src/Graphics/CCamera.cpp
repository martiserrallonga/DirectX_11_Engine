#include "CCamera.h"

void CCamera::SetProjectionValues(float aFov, float aAspect, float aNear, float aFar)
{
	float Fov = aFov * XM_PI / 180.f;
	mProjection = XMMatrixPerspectiveFovLH(Fov, aAspect, aNear, aFar);
}

