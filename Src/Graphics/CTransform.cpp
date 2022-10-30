#include "CTransform.h"

using namespace DirectX;

const XMVECTOR CTransform::ZERO = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR CTransform::RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR CTransform::UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR CTransform::FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

CTransform::CTransform(const XMMATRIX& aTransformMatrix)
	: mTransformMatrix(aTransformMatrix)
{}

void CTransform::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&mPos, pos);
	mVPos = pos;
	UpdateMatrix();
}

void CTransform::SetPosition(const XMFLOAT3& pos)
{
	mPos = pos;
	mVPos = XMLoadFloat3(&mPos);
	UpdateMatrix();
}

void CTransform::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

void CTransform::AddPosition(const XMVECTOR& pos)
{
	mVPos += pos;
	XMStoreFloat3(&mPos, mVPos);
	UpdateMatrix();
}

void CTransform::AddPosition(const XMFLOAT3& pos)
{
	AddPosition(pos.x, pos.y, pos.z);
}

void CTransform::AddPosition(float x, float y, float z)
{
	mPos.x += x;
	mPos.y += y;
	mPos.z += z;
	mVPos = XMLoadFloat3(&mPos);
	UpdateMatrix();
}

void CTransform::SetRotation(const XMVECTOR& rot)
{
	mVRot = rot;
	XMStoreFloat3(&mRot, rot);
	UpdateMatrix();
}

void CTransform::SetRotation(const XMFLOAT3& rot)
{
	mRot = rot;
	mVRot = XMLoadFloat3(&mRot);
	UpdateMatrix();
}

void CTransform::SetRotation(float x, float y, float z)
{
	SetRotation(XMFLOAT3(x, y, z));
}

void CTransform::AddRotation(const XMVECTOR& rot)
{
	mVRot += rot;
	XMStoreFloat3(&mRot, mVRot);
	UpdateMatrix();
}

void CTransform::AddRotation(const XMFLOAT3& rot)
{
	AddRotation(rot.x, rot.y, rot.z);
}

void CTransform::AddRotation(float x, float y, float z)
{
	mRot.x += x;
	mRot.y += y;
	mRot.z += z;
	mVRot = XMLoadFloat3(&mRot);
	UpdateMatrix();
}

void CTransform::LookAt(XMFLOAT3 pos)
{
	if (XMVector3Equal(mVPos, XMLoadFloat3(&pos))) return;

	pos.x -= mPos.x;
	pos.y -= mPos.y;
	pos.z -= mPos.z;

	float Distance = XMVectorGetX(XMVector2Length({ pos.x, pos.z }));
	float Pitch2 = atan2f(pos.y, Distance);
	float Yaw2 = atan2f(pos.x, pos.z);

	SetRotation(Pitch2, Yaw2, 0.f);
}

void CTransform::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{
	//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(FORWARD, camRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += mVPos;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(UP, camRotationMatrix);
	//Rebuild view matrix
	mTransformMatrix = XMMatrixLookAtLH(mVPos, camTarget, upDir);

	XMMATRIX YawRotationMatrix = XMMatrixRotationRollPitchYaw(0.f, mRot.y, 0.f);
	mForward = XMVector3TransformCoord(FORWARD, YawRotationMatrix);
	mRight = XMVector3TransformCoord(RIGHT, YawRotationMatrix);
}

void CTransform::UpdateWorldMatrix()
{
	mTransformMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z)
		* XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
	XMMATRIX YawRotationMatrix = XMMatrixRotationRollPitchYaw(0.f, mRot.y, 0.f);
	mForward = XMVector3TransformCoord(FORWARD, YawRotationMatrix);
	mRight = XMVector3TransformCoord(RIGHT, YawRotationMatrix);

}
