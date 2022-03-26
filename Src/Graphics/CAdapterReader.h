#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "CErrorLogger.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")


struct TAdapter
{
	explicit TAdapter(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC Description;
};

class CAdapterReader
{
public:
	static std::vector<TAdapter> GetAdapters();

private:
	static std::vector<TAdapter> mAdapters;
};