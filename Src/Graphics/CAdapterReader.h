#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "CErrorLogger.h"

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