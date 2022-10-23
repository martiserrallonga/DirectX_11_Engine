#pragma once
#include <string>

class CStringHelper
{
public:
	static std::wstring StringToWide(const std::string& aStr);
	static std::string GetDirectory(const std::string& aFilePath);
	static std::string GetFileExtension(const std::string& aFileName);

	static std::string ToLowercase(std::string&& str);
	static std::string ToLowercase(const std::string& str);

};

