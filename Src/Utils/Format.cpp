#include "Format.h"
#include "CErrorLogger.h"
#include <fstream>

// TODO: Add boolean for break execution if not found
json Format::JsonFile(std::string filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		CErrorLogger::Log(filename + " not found.");
		return {};
	}
	return json::parse(file);
}
