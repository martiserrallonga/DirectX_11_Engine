#include "CErrorLogger.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	HRESULT hr = S_OK; // You could try with S_OK, E_INVALIDARG, etc.
	
	if (SUCCEEDED(hr)) {
		MessageBoxA(NULL, "SUCCESS", "SUCCESS", NULL);
	}
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "FAILURE");
	}
	
	return 0;
}	