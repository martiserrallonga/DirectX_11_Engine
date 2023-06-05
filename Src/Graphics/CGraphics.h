#pragma once
#include "CWindow.h"
#include "CAdapterReader.h"
#include "CCamera.h"
#include "CLight.h"
#include "CTimer.h"
#include "Shaders.h"
#include "CGameEntity.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class CGraphics
{
public:
	bool Init(std::string configPath, const CWindow& window);
	void Render();

	CCamera Camera;

	CLight mLight;

	CGameEntity mSoldier;
	CGameEntity mBulb;

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
	CPixelShader mBulbPixelShader;
	CConstantBuffer<CBVertexShader> mCBVertexShader;
	CConstantBuffer<CBLight> mCBPixelShader;

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

	std::string _configPath;

public:
	bool mRotationMode = false;

};
