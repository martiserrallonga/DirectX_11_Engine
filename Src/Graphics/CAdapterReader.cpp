#include "CAdapterReader.h"

std::vector<TAdapter> CAdapterReader::mAdapters;

std::vector<TAdapter> CAdapterReader::GetAdapters()
{
	if (mAdapters.size() > 0) return mAdapters;

	// Initialize adapters
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;

	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter))) {
		mAdapters.emplace_back(pAdapter);
		index++;
	}

	return mAdapters;
}

TAdapter::TAdapter(IDXGIAdapter* aAdapter)
	: pAdapter(aAdapter)
{
	HRESULT hr = pAdapter->GetDesc(&Description);
	
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to Get Description for IDXGIAdapter.");
	}
}