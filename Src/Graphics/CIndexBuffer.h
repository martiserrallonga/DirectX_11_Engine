#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class CIndexBuffer
{
private:
	CIndexBuffer(const CIndexBuffer& rhs) = default;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
	UINT mBufferSize = 0;

public:
	CIndexBuffer() = default;

	HRESULT Init(ID3D11Device* aDevice, DWORD* aData, UINT aNumIndices)
	{
		mBufferSize = aNumIndices;
		
		D3D11_BUFFER_DESC IndexBufferDesc;
		ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = sizeof(DWORD) * aNumIndices;
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA IndexBufferData;
		IndexBufferData.pSysMem = aData;
		return aDevice->CreateBuffer(&IndexBufferDesc, &IndexBufferData, mBuffer.GetAddressOf());
	}

	ID3D11Buffer* Get() const {
		return mBuffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const {
		return mBuffer.GetAddressOf();
	}

	UINT GetBufferSize() const {
		return mBufferSize;
	}

};
