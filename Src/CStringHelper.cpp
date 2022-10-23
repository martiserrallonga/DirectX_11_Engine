#include "CStringHelper.h"
#include <vector>
#include <algorithm>

std::wstring CStringHelper::StringToWide(const std::string& aStr)
{
	return std::wstring(aStr.begin(), aStr.end());
}

std::string CStringHelper::GetDirectory(const std::string& aFilePath)
{
	std::vector<char> Separators{ '/', '\\' };
	size_t offset = 0;
	for (const auto& Separator : Separators) {
		auto off = aFilePath.find_last_of(Separator);
		if (off == std::string::npos) continue;
		offset = std::max(offset, off);
	}

	if (offset == 0) return {};
	return aFilePath.substr(0, offset);
}

std::string CStringHelper::GetFileExtension(const std::string& aFileName)
{
	auto offset = aFileName.find_last_of('.');
	if (offset == std::string::npos) return {};
	return ToLowercase(aFileName.substr(offset + 1));
}

std::string CStringHelper::ToLowercase(std::string&& str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](char c) { return std::tolower(c); });
	return str;
}

std::string CStringHelper::ToLowercase(const std::string& str)
{
	std::string Result(str);
	return ToLowercase(std::move(Result));
}