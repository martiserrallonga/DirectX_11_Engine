#include "CErrorLogger.h"
#include "CStringHelper.h"
#include <comdef.h>

void CErrorLogger::Log(std::string aMessage)
{
	aMessage = "Error: " + aMessage;
	MessageBoxA(NULL, aMessage.c_str(), "Error", MB_ICONERROR);
}

void CErrorLogger::Log(HRESULT hr, std::string aMessage)
{
	_com_error Error(hr);
	std::wstring Message = L"Error: " + CStringHelper::StringToWide(aMessage) + L'\n' + Error.ErrorMessage();
	MessageBoxW(NULL, Message.c_str(), L"Error", MB_ICONERROR);
}

void CErrorLogger::Log(HRESULT hr, std::wstring aMessage)
{
	_com_error Error(hr);
	std::wstring Message = L"Error: " + aMessage + L'\n' + Error.ErrorMessage();
	MessageBoxW(NULL, Message.c_str(), L"Error", MB_ICONERROR);
}

void CErrorLogger::Log(CComException& aException)
{
	std::wstring Message = aException.what();
	MessageBoxW(NULL, Message.c_str(), L"Error", MB_ICONERROR);
}
