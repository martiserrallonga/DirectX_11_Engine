#pragma once
#include "ConstantBufferTypes.h"
#include "CErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>

template<class T>
class CConstantBuffer
{
private:
	CConstantBuffer(const CConstantBuffer<T>& rhs) = default;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	ID3D11DeviceContext* mDeviceContext = nullptr;

public:
	T mData;

	CConstantBuffer() = default;

	ID3D11Buffer* Get() const {
		return mBuffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const {
		return mBuffer.GetAddressOf();
	}

	HRESULT Init(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
	{
		if (mBuffer.Get()) mBuffer.Reset();

		mDeviceContext = aDeviceContext;

		CD3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(CD3D11_BUFFER_DESC));
		UINT ByteSize = static_cast<UINT>(sizeof(T));
		UINT DummyBytes = 16 - ByteSize % 16;

		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.ByteWidth = ByteSize + DummyBytes;
		BufferDesc.StructureByteStride = 0;

		return aDevice->CreateBuffer(&BufferDesc, 0, mBuffer.GetAddressOf());
	}

	void Update()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		
		HRESULT hr = this->mDeviceContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		if (FAILED(hr)) {
			CErrorLogger::Log(hr, "Failed to map constant buffer.");
			return;
		}

		CopyMemory(MappedResource.pData, &mData, sizeof(T));
		mDeviceContext->Unmap(mBuffer.Get(), 0);
	}
};
