#include "CGraphics.h"

bool CGraphics::Init(HWND hwnd, int aWidth, int aHeight)
{
	Fps.Start();

	mWindowWidth = aWidth;
	mWindowHeight = aHeight;

	if (!InitDirectX(hwnd)) return false;
	if (!InitShaders()) return false;
	if (!InitScene()) return false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(mDevice.Get(), mDeviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void CGraphics::Render()
{
	float BackgroundColor[] = { 0.f, 0.f, 0.f, 1.f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), BackgroundColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	//mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	//mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	//mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->RSSetState(mRasterizerState.Get());
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mDeviceContext->OMSetBlendState(mBlendState.Get(), NULL, 0xFFFFFFFF);
	mDeviceContext->CSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	mDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
	mDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

	UINT Offset = 0;
	static float Alpha = 0.5f;

	// Pebble
	static float TranslationPebble[3] = { 0.f, 0.f, 1.f };
	{
		XMMATRIX World = XMMatrixTranslation(TranslationPebble[0], TranslationPebble[1], TranslationPebble[2]);
		XMMATRIX WVP = World * Camera.GetViewMatrix() * Camera.GetProjectionMatrix();
		mConstantBufferVSOffset.mData.Transform = XMMatrixTranspose(WVP);
		mConstantBufferVSOffset.Update();
		mDeviceContext->VSSetConstantBuffers(0, 1, mConstantBufferVSOffset.GetAddressOf());

		mConstantBufferPSBlending.mData.Alpha = 1.f;
		mConstantBufferPSBlending.Update();
		mDeviceContext->PSSetConstantBuffers(0, 1, mConstantBufferPSBlending.GetAddressOf());
		mDeviceContext->PSSetShaderResources(0, 1, mPebbleTexture.GetAddressOf());
		mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &Offset);
		mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		mDeviceContext->RSSetState(mRasterizerCullFrontState.Get());
		mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);
		mDeviceContext->RSSetState(mRasterizerState.Get());
		mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);	}

	// Grass
	static float TranslationGrass[3] = { 0.f, 0.f, -1.f };
	{
		XMMATRIX World = XMMatrixTranslation(TranslationGrass[0], TranslationGrass[1], TranslationGrass[2]);
		XMMATRIX WVP = World * Camera.GetViewMatrix() * Camera.GetProjectionMatrix();
		mConstantBufferVSOffset.mData.Transform = XMMatrixTranspose(WVP);
		mConstantBufferVSOffset.Update();
		mDeviceContext->VSSetConstantBuffers(0, 1, mConstantBufferVSOffset.GetAddressOf());

		mConstantBufferPSBlending.mData.Alpha = Alpha;
		mConstantBufferPSBlending.Update();
		mDeviceContext->PSSetConstantBuffers(0, 1, mConstantBufferPSBlending.GetAddressOf());
		mDeviceContext->PSSetShaderResources(0, 1, mGrassTexture.GetAddressOf());
		mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &Offset);
		mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		mDeviceContext->RSSetState(mRasterizerCullFrontState.Get());
		mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);
		mDeviceContext->RSSetState(mRasterizerState.Get());
		mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);	}

	// Marbled
	static float TranslationMarbled[3] = { 0.f, 0.f, 0.f };
	{
		XMMATRIX World = XMMatrixScaling(5.f, 5.f, 5.f) * XMMatrixTranslation(TranslationMarbled[0], TranslationMarbled[1], TranslationMarbled[2]);
		XMMATRIX WVP = World * Camera.GetViewMatrix() * Camera.GetProjectionMatrix();
		mConstantBufferVSOffset.mData.Transform = XMMatrixTranspose(WVP);
		mConstantBufferVSOffset.Update();
		mDeviceContext->VSSetConstantBuffers(0, 1, mConstantBufferVSOffset.GetAddressOf());

		mConstantBufferPSBlending.mData.Alpha = 1.f;
		mConstantBufferPSBlending.Update();
		mDeviceContext->PSSetConstantBuffers(0, 1, mConstantBufferPSBlending.GetAddressOf());
		mDeviceContext->PSSetShaderResources(0, 1, mMarbledTexture.GetAddressOf());
		mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &Offset);
		mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		mDeviceContext->RSSetState(mRasterizerCullFrontState.Get());
		mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);
		mDeviceContext->RSSetState(mRasterizerState.Get());
		mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);
	}




	// Text
	static int FpsCounter = 0;
	static std::string FpsString = "FPS: 0";
	FpsCounter++;
	if (Fps.GetMiliseconds() > 1000.f) {
		FpsString = "FPS: " + std::to_string(FpsCounter);
		FpsCounter = 0;
		Fps.Restart();
	}

	mSpriteBatch->Begin();
	mSpriteFont->DrawString(mSpriteBatch.get(),
		CStringConverter::StringToWide(FpsString).c_str(),
		DirectX::XMFLOAT2(0.f, 0.f), DirectX::Colors::White, 0.f,
		DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f));
	mSpriteBatch->End();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");

	static int Counter = 0;
	ImGui::Text("This is an example test!");
	if (ImGui::Button("CLICK ME")) Counter++;
	std::string CounterStr = "Click count: " + std::to_string(Counter);
	ImGui::SameLine();
	ImGui::Text(CounterStr.c_str());

	ImGui::DragFloat3("Grass Translation", TranslationGrass, 0.1f);
	ImGui::DragFloat3("Marbled Translation", TranslationMarbled, 0.1f);
	ImGui::DragFloat3("Pebble Translation", TranslationPebble, 0.1f);

	ImGui::DragFloat("Alpha", &Alpha, 0.01f, 0.f, 1.f);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	mSwapChain->Present(1, NULL);
}

bool CGraphics::InitDirectX(HWND hwnd)
{
	std::vector<TAdapter> Adapters = CAdapterReader::GetAdapters();
	if (Adapters.empty()) {
		CErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC Scd;
	ZeroMemory(&Scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	Scd.BufferDesc.Width = mWindowWidth;
	Scd.BufferDesc.Height = mWindowHeight;
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

	D3D11_TEXTURE2D_DESC DepthStencilBufferDesc;
	DepthStencilBufferDesc.Width = mWindowWidth;
	DepthStencilBufferDesc.Height = mWindowHeight;
	DepthStencilBufferDesc.MipLevels = 1;
	DepthStencilBufferDesc.ArraySize = 1;
	DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilBufferDesc.SampleDesc.Count = 1;
	DepthStencilBufferDesc.SampleDesc.Quality = 0;
	DepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilBufferDesc.CPUAccessFlags = 0;
	DepthStencilBufferDesc.MiscFlags = 0;

	hr = mDevice->CreateTexture2D(&DepthStencilBufferDesc, NULL, mDepthStencilBuffer.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
		return false;
	}

	hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), NULL, mDepthStencilView.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create depth stencil view.");
		return false;
	}

	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = mDevice->CreateDepthStencilState(&DepthStencilDesc, mDepthStencilState.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create depth stencil state.");
		return false;
	}

	D3D11_VIEWPORT Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = static_cast<float>(mWindowWidth);
	Viewport.Height = static_cast<float>(mWindowHeight);
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;

	mDeviceContext->RSSetViewports(1, &Viewport);

	D3D11_RASTERIZER_DESC RasterizerDesc;
	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	//RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	
	hr = mDevice->CreateRasterizerState(&RasterizerDesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create rasterizer state.");
		return false;
	}

	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

	hr = mDevice->CreateRasterizerState(&RasterizerDesc, mRasterizerCullFrontState.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create rasterizer state.");
		return false;
	}

	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));

	D3D11_RENDER_TARGET_BLEND_DESC RTBlendDesc;
	ZeroMemory(&RTBlendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

	RTBlendDesc.BlendEnable = true;
	RTBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	RTBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	RTBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	RTBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	RTBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	RTBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	RTBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	BlendDesc.RenderTarget[0] = RTBlendDesc;

	hr = mDevice->CreateBlendState(&BlendDesc, mBlendState.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create blend state.");
		return false;
	}

	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(mDeviceContext.Get());
	mSpriteFont = std::make_unique<DirectX::SpriteFont>(mDevice.Get(), L"Data/Fonts/comic_sans_ms_16.spritefont");

	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	hr = mDevice->CreateSamplerState(&SamplerDesc, mSamplerState.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create sampler state.");
		return false;
	}

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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, ipvd, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, ipvd, 0 },
	};
	
	if (!mVertexShader.Init(mDevice, ShaderFolder + L"VertexShader.cso", Layout, ARRAYSIZE(Layout))) return false;
	if (!mPixelShader.Init(mDevice, ShaderFolder + L"PixelShader.cso")) return false;

	return true;
}

bool CGraphics::InitScene()
{
	TVertex Vertex []
	{
		TVertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
		TVertex(-0.5f, +0.5f, -0.5f, 0.0f, 0.0f),
		TVertex(+0.5f, +0.5f, -0.5f, 1.0f, 0.0f),
		TVertex(+0.5f, -0.5f, -0.5f, 1.0f, 1.0f),

		TVertex(-0.5f, -0.5f, +0.5f, 0.0f, 1.0f),
		TVertex(-0.5f, +0.5f, +0.5f, 0.0f, 0.0f),
		TVertex(+0.5f, +0.5f, +0.5f, 1.0f, 0.0f),
		TVertex(+0.5f, -0.5f, +0.5f, 1.0f, 1.0f),
	};

	HRESULT hr = mVertexBuffer.Init(mDevice.Get(), Vertex, ARRAYSIZE(Vertex));
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	DWORD Index[]
	{
		0,1,2,
		0,2,3,
		1,5,6,
		1,6,2,
		2,6,7,
		2,7,3,
		4,0,3,
		4,3,7,
		1,0,4,
		1,4,5,
		4,7,6,
		4,6,5,
	};

	hr = mIndexBuffer.Init(mDevice.Get(), Index, ARRAYSIZE(Index));
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create index buffer.");
		return false;
	}

	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to call CoInitialize.");
		return false;
	}


	// Textures
	hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data/Textures/grass.jpg", nullptr, mGrassTexture.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create wic texture from file.");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data/Textures/marbled.jpg", nullptr, mMarbledTexture.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create wic texture from file.");
		return false;
	}
	
	hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data/Textures/pebble.jpg", nullptr, mPebbleTexture.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create wic texture from file.");
		return false;
	}



	hr = mConstantBufferVSOffset.Init(mDevice.Get(), mDeviceContext.Get());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to initialize constant buffer.");
		return false;
	}

	hr = mConstantBufferPSBlending.Init(mDevice.Get(), mDeviceContext.Get());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to initialize constant buffer.");
		return false;
	}

	Camera.SetPosition(0.f, 0.f, -4.f);

	float Fov = 90.f;
	float AspectRatio = static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight);
	float NearZ = 0.1f;
	float FarZ = 1000.f;
	Camera.SetProjectionValues(Fov, AspectRatio, NearZ, FarZ);

	return true;
}
