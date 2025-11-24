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
	NotSupportedCompressionMode
};

ZIPLIB_API std::string GetZIPErrorMsg(ZIPErrorCode ec);
