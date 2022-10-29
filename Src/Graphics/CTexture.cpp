#include "CTexture.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"


CTexture::CTexture(ID3D11Device* aDevice, aiTextureType aType, const CColor4& aColor)
	: mType(aType) {
	InitColorTexture(aDevice, &aColor, 1, 1);
}

CTexture::CTexture(ID3D11Device* aDevice, aiTextureType aType, const CColor4* aColorData, UINT width, UINT height)
	: mType(aType) {
	InitColorTexture(aDevice, aColorData, width, height);
}

CTexture::CTexture(ID3D11Device* aDevice, aiTextureType aType, const std::string& aFilePath)
	: mType(aType) {
	ID3D11Texture2D* pTexture = nullptr;

	HRESULT hr;
	if (CStringHelper::GetFileExtension(aFilePath) == "dds") {
		hr = DirectX::CreateWICTextureFromFile(aDevice, CStringHelper::StringToWide(aFilePath).c_str(), mTexture.GetAddressOf(), mTextureView.GetAddressOf());
		if (FAILED(hr)) InitColorTexture(aDevice, &color::UnloadedTextureColor, 1, 1);
	}
	else {
		hr = DirectX::CreateWICTextureFromFile(aDevice, CStringHelper::StringToWide(aFilePath).c_str(), mTexture.GetAddressOf(), mTextureView.GetAddressOf());
		if (FAILED(hr)) InitColorTexture(aDevice, &color::UnloadedTextureColor, 1, 1);
	}
}

CTexture::CTexture(ID3D11Device* aDevice, aiTextureType aType, const aiTexel* aTexelData, size_t aSize)
	: mType (aType) {
	HRESULT hr = DirectX::CreateWICTextureFromMemory(aDevice, reinterpret_cast<const uint8_t*>(aTexelData), aSize, mTexture.GetAddressOf(), mTextureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create Texture from memory.");
}

void CTexture::InitColorTexture(ID3D11Device* aDevice, const CColor4* aColorData, UINT width, UINT height)
{
	CD3D11_TEXTURE2D_DESC TextureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_SUBRESOURCE_DATA ResourceData{};

	ResourceData.pSysMem = aColorData;
	ResourceData.SysMemPitch = width * sizeof(CColor4);
	
	HRESULT hr;
	hr = aDevice->CreateTexture2D(&TextureDesc, &ResourceData, &pTexture);
	COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from color data.");
	
	mTexture = dynamic_cast<ID3D11Texture2D*>(pTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc(D3D11_SRV_DIMENSION_TEXTURE2D, TextureDesc.Format);

	hr = aDevice->CreateShaderResourceView(mTexture.Get(), &SRVDesc, mTextureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from texture generated from color data.");
}
