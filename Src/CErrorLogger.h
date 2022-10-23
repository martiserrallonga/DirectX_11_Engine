#pragma once
#include "CComException.h"
#include <Windows.h>

class CErrorLogger
{
public:
	static void Log(std::string aMessage);
	static void Log(HRESULT hr, std::string aMessage);
	static void Log(HRESULT hr, std::wstring aMessage);
	static void Log(CComException& aException);

};

