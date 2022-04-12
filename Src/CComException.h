#pragma once
#include "CStringConverter.h"
#include <comdef.h>

#define COM_ERROR_IF_FAILED( hr, msg ) if(FAILED(hr)) \
	throw CComException(hr, msg, __FILE__, __FUNCTION__, __LINE__)

class CComException
{
public:
	CComException(HRESULT hr, const std::string& Msg, const std::string& File, const std::string& Function, int Line)
	{
		_com_error error(hr);
		whatmsg = L"Msg: " + CStringConverter::StringToWide(std::string(Msg)) + L"\n";
		whatmsg += error.ErrorMessage();
		whatmsg += L"\nFile: " + CStringConverter::StringToWide(File);
		whatmsg += L"\nFunction: " + CStringConverter::StringToWide(Function);
		whatmsg += L"\nLine: " + CStringConverter::StringToWide(std::to_string(Line));
	}

	const wchar_t* what() const { return whatmsg.c_str(); }

private:
	std::wstring whatmsg;

};