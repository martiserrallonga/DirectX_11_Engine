#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class CTransform
{
public:
	static const XMVECTOR ZERO;
	static const XMVECTOR RIGHT;
	static const XMVECTOR UP;
	static const XMVECTOR FORWARD;

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

protected:
	virtual void UpdateMatrix() = 0;
	void UpdateViewMatrix();
	void UpdateWorldMatrix();

private:
	XMVECTOR mVPos;
	XMVECTOR mVRot;
	XMFLOAT3 mPos;
	XMFLOAT3 mRot;

	XMVECTOR mForward;
	XMVECTOR mRight;

	XMMATRIX mTransformMatrix;

};

