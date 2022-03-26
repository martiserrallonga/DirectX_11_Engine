#include "CGraphics.h"

bool CGraphics::Init(HWND hwnd, int width, int height)
{
	if (!InitDirectX(hwnd, width, height)) return false;
	return true;
}

void CGraphics::Render()
{
	float BackgroundColor[] = { 0.f, 0.f, 1.f, 1.f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), BackgroundColor);
	mSwapChain->Present(1, NULL);
}

bool CGraphics::InitDirectX(HWND hwnd, int width, int height)
{
	std::vector<TAdapter> Adapters = CAdapterReader::GetAdapters();
	if (Adapters.empty()) {
		CErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC Scd;
	ZeroMemory(&Scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	Scd.BufferDesc.Width = width;
	Scd.BufferDesc.Height = height;
	Scd.BufferDesc.RefreshRate.Numerator = 60;
	Scd.BufferDesc.RefreshRate.Denominator = 1;
	Scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	Scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	Scd.SampleDesc.Count = 1;
	Scd.SampleDesc.Quality = 0;
	
	Scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	Scd.BufferCount = 1;
	Scd.OutputWindow = hwnd;
	Scd.Windowed = TRUE;
	Scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	Scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		Adapters.front().pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,		// For Software Driver Type
		NULL,		// Flags for Runtime Layers
		NULL,		// Features Levels Array
		0,			// # Fetures Levels in Array
		D3D11_SDK_VERSION,
		&Scd,
		mSwapChain.GetAddressOf(),
		mDevice.GetAddressOf(),
		NULL,		// Suported Feature Level
		mDeviceContext.GetAddressOf()
	);

	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuffer.GetAddressOf()));
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "GetBuffer Failed.");
		return false;
	}

	hr = mDevice->CreateRenderTargetView(BackBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
	
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), NULL);

	return true;
}
