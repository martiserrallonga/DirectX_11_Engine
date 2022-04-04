#pragma once
#include "CAdapterReader.h"
#include "Shaders.h"

#include <SpriteBatch.h>
#include <SpriteFont.h>

#include <WICTextureLoader.h>

#include "TVertex.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

#include "CConstantBuffer.h"

#include "CCamera.h"

class CGraphics
{
public:
	bool Init(HWND hwnd, int aWidth, int aHeight);
	void Render();

	CCamera Camera;

private:
	bool InitDirectX(HWND hwnd);
	bool InitShaders();
	bool InitScene();

	int mWindowWidth = 0;
	int mWindowHeight = 0;

	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

	CVertexShader mVertexShader;
	CPixelShader mPixelShader;
	CConstantBuffer<CB_VS_Offset> mConstantBuffer;

	CVertexBuffer<TVertex> mVertexBuffer;
	CIndexBuffer mIndexBuffer;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
	
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

};
