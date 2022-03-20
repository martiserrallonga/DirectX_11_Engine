#pragma once
#include "CStringConverter.h"
#include <Windows.h>

class CErrorLogger
{
public:
	static void Log(std::string aMessage);
	static void Log(HRESULT hr, std::string aMessage);

};

