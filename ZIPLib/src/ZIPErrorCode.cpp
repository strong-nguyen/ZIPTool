#include "pch.h"
#include "../include/ZIPErrorCode.h"
#include <unordered_map>
#include <format>

static const std::unordered_map<ZIPErrorCode, std::string> EcTable = {
	{ZIPErrorCode::Success, "Success"},
	{ZIPErrorCode::ZipFileNotExisted, "Input file is not found"},
	{ZIPErrorCode::OutputDirNotExisted, "Output dir is not existed"},
	{ZIPErrorCode::ZipFileOpenFailed, "Failed to open zip file"},
	{ZIPErrorCode::EOCDNotFound, "Cannot find EOCD"},
	{ZIPErrorCode::InvalidCentralDirectorySignature, "Signature of CD not matched with ZIP spec"},
	{ZIPErrorCode::InvalidLocalFileHeaderSignature, "Signature of Local File Header not matched with ZIP spec"},
	{ZIPErrorCode::NotSupportedCompressionMode, "Compression mode is not supported"},
	{ZIPErrorCode::CreateDirectoryFailed, "Failed to create directory when extract zip file"},
	{ZIPErrorCode::DecompressionFailed, "Deflate decompress failed"}
};

std::string GetZIPErrorMsg(ZIPErrorCode ec)
{
	auto it = EcTable.find(ec);
	return it != EcTable.end() ? it->second : std::format("Not defined ec: {}", static_cast<int>(ec));
}