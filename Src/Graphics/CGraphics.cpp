#include "CGraphics.h"
#include "CComException.h"

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
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->RSSetState(mRasterizerState.Get());
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mDeviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	mDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
	mDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

	mSoldier.Render(Camera.GetViewProjectionMatrix());

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
		CStringHelper::StringToWide(FpsString).c_str(),
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

	ImGui::InputFloat("Camera Speed", &Camera.Speed, 0.f, 0.f, "%.1f");

	ImGui::Checkbox("Microwave", &mRotationMode);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	mSwapChain->Present(1, NULL);
}

bool CGraphics::InitDirectX(HWND hwnd)
{
	try
	{
		std::vector<TAdapter> Adapters = CAdapterReader::GetAdapters();
		if (Adapters.empty()) {
			CErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC Scd{};
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
		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "GetBuffer Failed.");

		hr = mDevice->CreateRenderTargetView(BackBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

		CD3D11_TEXTURE2D_DESC DepthStencilBufferDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, mWindowWidth, mWindowHeight);
		DepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthStencilBufferDesc.MipLevels = 1;

		hr = mDevice->CreateTexture2D(&DepthStencilBufferDesc, NULL, mDepthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), NULL, mDepthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		CD3D11_DEPTH_STENCIL_DESC DepthStencilDesc(D3D11_DEFAULT);
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		hr = mDevice->CreateDepthStencilState(&DepthStencilDesc, mDepthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		CD3D11_VIEWPORT Viewport(0.f, 0.f, static_cast<float>(mWindowWidth), static_cast<float>(mWindowHeight));
		mDeviceContext->RSSetViewports(1, &Viewport);

		CD3D11_RASTERIZER_DESC RasterizerDesc(D3D11_DEFAULT);
		hr = mDevice->CreateRasterizerState(&RasterizerDesc, mRasterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		hr = mDevice->CreateRasterizerState(&RasterizerDesc, mRasterizerCullFrontState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		D3D11_BLEND_DESC BlendDesc{};
		D3D11_RENDER_TARGET_BLEND_DESC RTBlendDesc{};
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
		COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

		mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(mDeviceContext.Get());
		mSpriteFont = std::make_unique<DirectX::SpriteFont>(mDevice.Get(), L"Data/Fonts/comic_sans_ms_16.spritefont");

		CD3D11_SAMPLER_DESC SamplerDesc(D3D11_DEFAULT);
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		//SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		//SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		//SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		//SamplerDesc.BorderColor[0] = 0.8f;
		//SamplerDesc.BorderColor[1] = 0.2f;
		//SamplerDesc.BorderColor[2] = 0.2f;
		//SamplerDesc.BorderColor[3] = 1.0f;

		hr = mDevice->CreateSamplerState(&SamplerDesc, mSamplerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
	}
	catch (CComException& Exception)
	{
		CErrorLogger::Log(Exception);
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
	try
	{
		HRESULT hr = CoInitialize(NULL);
		COM_ERROR_IF_FAILED(hr, "Failed to call CoInitialize.");

		hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data/Textures/grass.jpg", nullptr, mGrassTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data/Textures/marbled.jpg", nullptr, mMarbledTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data/Textures/pebble.jpg", nullptr, mPebbleTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = mCBVertexShader.Init(mDevice.Get(), mDeviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = mCBPixelShader.Init(mDevice.Get(), mDeviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		//if (!mSoldier.Init("Data/Objects/Samples/blue_cube_notexture.fbx", mDevice.Get(), mDeviceContext.Get(), mCBVertexShader))
		//if (!mSoldier.Init("Data/Objects/Nanosuit/Nanosuit.obj", mDevice.Get(), mDeviceContext.Get(), mCBVertexShader))
		//if (!mSoldier.Init("Data/Objects/Samples/orange_disktexture.fbx", mDevice.Get(), mDeviceContext.Get(), mCBVertexShader))
		//if (!mSoldier.Init("Data/Objects/Samples/orange_embeddedtexture.fbx", mDevice.Get(), mDeviceContext.Get(), mCBVertexShader))
		//if (!mSoldier.Init("Data/Objects/Samples/person_embeddedindexed.blend", mDevice.Get(), mDeviceContext.Get(), mCBVertexShader))
		if (!mSoldier.Init("Data/Objects/Samples/dodge_challenger.fbx", mDevice.Get(), mDeviceContext.Get(), mCBVertexShader))
			return false;

		float Fov = 90.f;
		float AspectRatio = static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight);
		float NearZ = 0.1f;
		float FarZ = 1000.f;
		Camera.SetProjectionValues(Fov, AspectRatio, NearZ, FarZ);
		Camera.SetPosition(0.f, 0.f, -4.f);

	}
	catch (CComException& Exception)
	{
		CErrorLogger::Log(Exception);
		return false;
	}
	return true;
}
