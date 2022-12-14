#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class CTransform
{
	friend class CCamera;

public:
	CTransform() = default;
	explicit CTransform(const XMMATRIX& aTransformMatrix);

	const XMVECTOR& GetPositionVector() const { return mVPos; }
	const XMFLOAT3& GetPositionFloat3() const { return mPos; }
	const XMVECTOR& GetRotationVector() const { return mVRot; }
	const XMFLOAT3& GetRotationFloat3() const { return mRot; }

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);

	void AddPosition(const XMVECTOR& pos);
	void AddPosition(const XMFLOAT3& pos);
	void AddPosition(float x, float y, float z);

	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);

	void AddRotation(const XMVECTOR& rot);
	void AddRotation(const XMFLOAT3& rot);
	void AddRotation(float x, float y, float z);

	void LookAt(XMFLOAT3 pos);

	const XMVECTOR& GetForward() const { return mForward; }
	const XMVECTOR& GetRight() const { return mRight; }

	const XMMATRIX& GetTransformMatrix() const { return mTransformMatrix; }
	void SetTransformMatrix(const XMMATRIX& aTransformMatrix) { mTransformMatrix = aTransformMatrix; }


private:
	void UpdateVectorsFromYaw();
	virtual void UpdateMatrix();


	XMVECTOR mVPos;
	XMVECTOR mVRot;
	XMFLOAT3 mPos;
	XMFLOAT3 mRot;

	XMVECTOR mForward;
	XMVECTOR mRight;

	XMMATRIX mTransformMatrix;

};

namespace vec {
	const XMVECTOR ZERO = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
}
