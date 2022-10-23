#include "CTexture.h"


CTexture::CTexture(ID3D11Device* aDevice, aiTextureType aType, const CColor4& colorData)
	: CTexture(aDevice, aType, &colorData, 1, 1)
{}

CTexture::CTexture(ID3D11Device* aDevice, aiTextureType aType, const CColor4* colorData, UINT width, UINT height)
	: mType(aType)
{
	CD3D11_TEXTURE2D_DESC TextureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_SUBRESOURCE_DATA ResourceData{};
	
	ResourceData.pSysMem = colorData;
	ResourceData.SysMemPitch = width * sizeof(CColor4);

	HRESULT hr = aDevice->CreateTexture2D(&TextureDesc, &ResourceData, &pTexture);
	COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from color data.");
	mTexture = dynamic_cast<ID3D11Texture2D*>(pTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc(D3D11_SRV_DIMENSION_TEXTURE2D, TextureDesc.Format);

	hr = aDevice->CreateShaderResourceView(mTexture.Get(), &SRVDesc, mTextureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from texture generated from color data.");
}
