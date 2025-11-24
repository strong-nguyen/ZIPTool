#pragma once

#include "ZIPDefine.h"

enum ZIPError
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

ZIPLIB_API std::string GetZIPErrorMsg(ZIPError ec);
