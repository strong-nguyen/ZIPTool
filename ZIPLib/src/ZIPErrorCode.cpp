#include "pch.h"
#include "../include/ZIPErrorCode.h"
#include <unordered_map>

static const std::unordered_map<ZIPErrorCode, std::string> EcTable = {
	{ZIPErrorCode::Success, "Success"},
	{ZIPErrorCode::ZipFileNotExisted, "Input file is not zip file"},
	{ZIPErrorCode::OutputDirNotExisted, "Output dir is not existed"},
	{ZIPErrorCode::ZipFileOpenFailed, "Failed to open zip file"}
};

std::string GetZIPErrorMsg(ZIPErrorCode ec)
{
	auto it = EcTable.find(ec);
	return it != EcTable.end() ? it->second : std::format("Not defined ec: {}", static_cast<int>(ec));
}