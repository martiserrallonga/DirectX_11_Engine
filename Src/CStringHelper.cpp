#include "CStringHelper.h"

std::wstring CStringHelper::StringToWide(std::string aStr)
{
	return std::wstring(aStr.begin(), aStr.end());
}
