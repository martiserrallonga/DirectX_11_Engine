#include "CGraphics.h"

bool CGraphics::Init(HWND hwnd, int width, int height)
{
	if (!InitDirectX(hwnd, width, height)) return false;
	return true;
}

bool CGraphics::InitDirectX(HWND hwnd, int width, int height)
{
	std::vector<TAdapter> Adapters = CAdapterReader::GetAdapters();
	//HRESULT hr = D3D11CreateDeviceAndSwapChain( )
	return true;
}
