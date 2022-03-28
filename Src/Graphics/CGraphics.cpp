#include "CGraphics.h"
#include "TVertex.h"

bool CGraphics::Init(HWND hwnd, int aWidth, int aHeight)
{
	if (!InitDirectX(hwnd, aWidth, aHeight)) return false;
	if (!InitShaders()) return false;
	if (!InitScene()) return false;

	return true;
}

void CGraphics::Render()
{
	float BackgroundColor[] = { 0.f, 0.f, 0.f, 1.f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), BackgroundColor);

	mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	//mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	//mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	//mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
	mDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

	UINT Stride = sizeof(TVertex);
	UINT Offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &Stride, &Offset);

	mDeviceContext->Draw(3, 0);

	mSwapChain->Present(1, NULL);
}

bool CGraphics::InitDirectX(HWND hwnd, int aWidth, int aHeight)
{
	std::vector<TAdapter> Adapters = CAdapterReader::GetAdapters();
	if (Adapters.empty()) {
		CErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC Scd;
	ZeroMemory(&Scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	Scd.BufferDesc.Width = aWidth;
	Scd.BufferDesc.Height = aHeight;
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

	D3D11_VIEWPORT Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = aWidth;
	Viewport.Height = aHeight;
	mDeviceContext->RSSetViewports(1, &Viewport);


	return true;
}

bool CGraphics::InitShaders()
{
	std::wstring ShaderFolder;

#pragma region GetShaderFolder
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _X86_
		ShaderFolder = L"Build/Win32/";
#else
		ShaderFolder = L"Build/x64/";
#endif

#ifdef _DEBUG
		ShaderFolder += L"Debug/";
#else
		ShaderFolder += L"Release/";
#endif
	}
#pragma endregion

	// Take a look to DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT
	auto ipvd = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	D3D11_INPUT_ELEMENT_DESC Layout[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, ipvd, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, ipvd, 0 },
	};
	
	if (!mVertexShader.Init(mDevice, ShaderFolder + L"VertexShader.cso", Layout, ARRAYSIZE(Layout))) return false;
	if (!mPixelShader.Init(mDevice, ShaderFolder + L"PixelShader.cso")) return false;

	return true;
}

bool CGraphics::InitScene()
{
	TVertex v[] =
	{
		TVertex(-0.5f, -0.5f, 1.f, 0.f, 0.f),
		TVertex(+0.0f, +0.5f, 0.f, 1.f, 0.f),
		TVertex(+0.5f, -0.5f, 0.f, 0.f, 1.f),
	};

	D3D11_BUFFER_DESC VertexBufferDescr;
	ZeroMemory(&VertexBufferDescr, sizeof(D3D11_BUFFER_DESC));

	VertexBufferDescr.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDescr.ByteWidth = sizeof(TVertex) * ARRAYSIZE(v);
	VertexBufferDescr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDescr.CPUAccessFlags = 0;
	VertexBufferDescr.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA VertexBufferData;
	ZeroMemory(&VertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	VertexBufferData.pSysMem = v;

	HRESULT hr = mDevice->CreateBuffer(&VertexBufferDescr, &VertexBufferData, mVertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	return true;
}
