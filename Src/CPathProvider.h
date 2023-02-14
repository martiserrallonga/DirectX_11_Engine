#pragma once

#include "CStringHelper.h"
#include <string>

class CPathProvider
{
public:

	static std::string GetData(std::string aStr) {
		return std::string(SOLUTION_DIR) + "Data/" + aStr;
	}

	static std::wstring WGetData(std::string aStr) {
		return CStringHelper::StringToWide(GetData(aStr));
	}

};