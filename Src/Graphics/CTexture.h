#pragma once
#include "CColor4.h"
#include "../CComException.h"
#include <d3d11.h>
#include <assimp/material.h>
#include <wrl/client.h>

enum class ETextureStorageType
{
	None,
	Invalid,
	Normal,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk,
};

class CTexture
{
public:
	CTexture(ID3D11Device* aDevice, aiTextureType aType, const CColor4& aColor);
	CTexture(ID3D11Device* aDevice, aiTextureType aType, const CColor4* aColorData, UINT width, UINT height);
	CTexture(ID3D11Device* aDevice, aiTextureType aType, const std::string& aFilePath);

	void InitColorTexture(ID3D11Device* aDevice, const CColor4* aColorData, UINT width, UINT height);

	aiTextureType GetType() const { return mType; }

	ID3D11ShaderResourceView* GetTextureView() const { return mTextureView.Get(); }
	ID3D11ShaderResourceView* const* GetTextureViewAdress() const { return mTextureView.GetAddressOf(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Resource> mTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureView;

	aiTextureType mType = aiTextureType::aiTextureType_UNKNOWN;

};

