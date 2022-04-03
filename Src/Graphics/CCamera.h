#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class CCamera
{
public:
	CCamera();

	void SetProjectionValues(float aFov, float aAspect, float aNear, float aFar) {
		float Fov = aFov * XM_PI / 180.f;
		mProjection = XMMatrixPerspectiveFovLH(Fov, aAspect, aNear, aFar);
	}

	const XMMATRIX& GetViewMatrix() const { return mView; }
	const XMMATRIX& GetProjectionMatrix() const { return mProjection; }
	const XMVECTOR& GetPositionVector() const { return mVPos; }
	const XMFLOAT3& GetPositionFloat3() const { return mPos; }
	const XMVECTOR& GetRotationVector() const { return mVRot; }
	const XMFLOAT3& GetRotationFloat3() const { return mRot; }

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AddPosition(const XMVECTOR& pos);
	void AddPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AddRotation(const XMVECTOR& rot);
	void AddRotation(float x, float y, float z);

private:
	void UpdateViewMatrix();
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMVECTOR mVPos;
	XMVECTOR mVRot;
	XMFLOAT3 mPos;
	XMFLOAT3 mRot;

	static const XMVECTOR DEFAULT_FORWARD_VECTOR;
	static const XMVECTOR DEFAULT_UP_VECTOR;

};
