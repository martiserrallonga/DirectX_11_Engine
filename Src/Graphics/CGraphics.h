#pragma once
#include "CAdapterReader.h"
#include "Shaders.h"

class CGraphics
{
public:
	bool Init(HWND hwnd, int aWidth, int aHeight);
	void Render();

private:
	bool InitDirectX(HWND hwnd, int aWidth, int aHeight);
	bool InitShaders();
	bool InitScene();

	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

	CVertexShader mVertexShader;
	CPixelShader mPixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
};
