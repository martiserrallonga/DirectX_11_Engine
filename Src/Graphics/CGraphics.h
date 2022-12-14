#pragma once
#include "CAdapterReader.h"
#include "CCamera.h"
#include "CTimer.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "Shaders.h"
#include "CGameEntity.h"

#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>


class CGraphics
{
public:
	bool Init(HWND hwnd, int aWidth, int aHeight);
	void Render();

	CCamera Camera;
	CGameEntity mSoldier;

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
	CConstantBuffer<CBVertexShader> mCBVertexShader;
	CConstantBuffer<CBAmbientLight> mCBPixelShader;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerCullFrontState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
	
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mGrassTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mMarbledTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mPebbleTexture;

	CTimer Fps;

public:
	bool mRotationMode = false;

};
