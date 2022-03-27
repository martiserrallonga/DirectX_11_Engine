#pragma once
#include "CAdapterReader.h"
#include "Shaders.h"

class CGraphics
{
public:
	bool Init(HWND hwnd, int width, int height);
	void Render();

private:
	bool InitDirectX(HWND hwnd, int width, int height);
	bool InitShaders();

	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	CVertexShader mVertexShader;

};
