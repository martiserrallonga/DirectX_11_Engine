#include "CCamera.h"

void CCamera::SetProjectionValues(float aFov, float aAspect, float aNear, float aFar) {
	float Fov = aFov * XM_PI / 180.f;
	mProjection = XMMatrixPerspectiveFovLH(Fov, aAspect, aNear, aFar);
}

const XMMATRIX& CCamera::GetViewProjectionMatrix() const {
	return GetTransformMatrix() * mProjection;
}

void CCamera::UpdateMatrix() {
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z);
	XMVECTOR Front = XMVector3TransformCoord(vec::FORWARD, RotationMatrix) + mVPos;
	XMVECTOR Up = XMVector3TransformCoord(vec::UP, RotationMatrix);
	SetTransformMatrix(XMMatrixLookAtLH(mVPos, Front, Up));
	UpdateVectorsFromYaw();
}
