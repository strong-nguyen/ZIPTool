#include "pch.h"
#include "../include/ZIPError.h"
#include <unordered_map>

static const std::unordered_map<ZIPError, std::string> EcTable = {
	{ZIPError::Success, "Success"},
	{ZIPError::ZipFileNotExisted, "Input file is not zip file"},
	{ZIPError::OutputDirNotExisted, "Output dir is not existed"},
	{ZIPError::ZipFileOpenFailed, "Failed to open zip file"}
};

std::string GetZIPErrorMsg(ZIPError ec)
{
	auto it = EcTable.find(ec);
	return it != EcTable.end() ? it->second : std::format("Not defined ec: {}", static_cast<int>(ec));
}