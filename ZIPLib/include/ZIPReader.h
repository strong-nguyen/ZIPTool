#pragma once

#include "ZIPDefine.h"
#include "ZIPError.h"

#include <memory>


class ZIPReaderImpl;

class ZIPLIB_API ZIPReader
{
public:
	ZIPReader(const std::string& zip_path, const std::string& output_path);

	~ZIPReader();

	ZIPError UnZip();

private:
	std::unique_ptr<ZIPReaderImpl> pimpl;  // Hide the detail implementation for user
};

