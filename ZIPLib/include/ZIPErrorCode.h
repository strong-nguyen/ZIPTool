#pragma once

#include "ZIPDefine.h"

enum ZIPErrorCode
{
	Success = 0,
	ZipFileNotExisted,
	OutputDirNotExisted,
	ZipFileOpenFailed,
	EOCDNotFound,
	InvalidCentralDirectorySignature,
	InvalidLocalFileHeaderSignature,
	NotSupportedCompressionMode,
	CreateDirectoryFailed,
	DecompressionFailed
};

ZIPLIB_API std::string GetZIPErrorMsg(ZIPErrorCode ec);
