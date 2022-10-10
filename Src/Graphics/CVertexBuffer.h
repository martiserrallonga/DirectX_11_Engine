#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>
class CVertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	UINT mStride = sizeof(T);
	UINT mVertexCount = 0;

public:
	CVertexBuffer() = default;

	HRESULT Init(ID3D11Device* aDevice, T* aData, UINT aVertexCount)
	{
		if (mBuffer.Get()) mBuffer.Reset();
		mVertexCount = aVertexCount;

		D3D11_BUFFER_DESC VertexBufferDesc;
		ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));

		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = sizeof(T) * aVertexCount;
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA VertexBufferData;
		ZeroMemory(&VertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		VertexBufferData.pSysMem = aData;

		return aDevice->CreateBuffer(&VertexBufferDesc, &VertexBufferData, mBuffer.GetAddressOf());
	}
	ID3D11Buffer* Get() const {
		return mBuffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const {
		return mBuffer.GetAddressOf();
	}

	UINT GetVertexCount() const {
		return mVertexCount;
	}

	const UINT GetStride() const {
		return mStride;
	}

	const UINT* GetStridePtr() const {
		return &mStride;
	}

};
