#include "CCamera.h"

const XMVECTOR CCamera::DEFAULT_RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR CCamera::DEFAULT_UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR CCamera::DEFAULT_FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

CCamera::CCamera()
	: mPos(0.f, 0.f, 0.f)
	, mVPos(XMLoadFloat3(&mPos))
	, mRot(0.f, 0.f, 0.f)
	, mVRot(XMLoadFloat3(&mRot))
{
	UpdateViewMatrix();
}

void CCamera::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&mPos, pos);
	mVPos = pos;
	UpdateViewMatrix();
}

void CCamera::SetPosition(float x, float y, float z)
{
	mPos = XMFLOAT3(x, y, z);
	mVPos = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void CCamera::AddPosition(const XMVECTOR& pos)
{
	mVPos += pos;
	XMStoreFloat3(&mPos, mVPos);
	UpdateViewMatrix();
}

void CCamera::AddPosition(float x, float y, float z)
{
	mPos.x += x;
	mPos.y += y;
	mPos.z += z;
	mVPos = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void CCamera::SetRotation(const XMVECTOR& rot)
{
	mVRot = rot;
	XMStoreFloat3(&mRot, rot);
	UpdateViewMatrix();
}

void CCamera::SetRotation(float x, float y, float z)
{
	mRot = XMFLOAT3(x, y, z);
	mVRot = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void CCamera::AddRotation(const XMVECTOR& rot)
{
	mVRot += rot;
	XMStoreFloat3(&mRot, mVRot);
	UpdateViewMatrix();
}

void CCamera::AddRotation(float x, float y, float z)
{
	mRot.x += x;
	mRot.y += y;
	mRot.z += z;
	mVRot = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void CCamera::LookAt(XMFLOAT3 pos)
{
	if (XMVector3Equal(mVPos, XMLoadFloat3(&pos))) return;

	pos.x -= mPos.x;
	pos.y -= mPos.y;
	pos.z -= mPos.z;

	float Distance = XMVectorGetX(XMVector2Length({ pos.x, pos.z }));
	float Pitch2 = atan2(pos.y, Distance);
	float Yaw2 = atan2(pos.x, pos.z);

	SetRotation(Pitch2, Yaw2, 0.f);
}

void CCamera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{
	//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD, camRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += mVPos;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP, camRotationMatrix);
	//Rebuild view matrix
	mView = XMMatrixLookAtLH(mVPos, camTarget, upDir);

	XMMATRIX YawRotationMatrix = XMMatrixRotationRollPitchYaw(0.f, mRot.y, 0.f);
	mForward = XMVector3TransformCoord(DEFAULT_FORWARD, YawRotationMatrix);
	mRight = XMVector3TransformCoord(DEFAULT_RIGHT, YawRotationMatrix);

}
