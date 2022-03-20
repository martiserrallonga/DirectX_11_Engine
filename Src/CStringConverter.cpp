#include "CStringConverter.h"

std::wstring CStringConverter::StringToWide(std::string aStr)
{
	return std::wstring(aStr.begin(), aStr.end());
}
