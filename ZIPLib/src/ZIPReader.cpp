#include "pch.h"
#include "../include/ZIPReader.h"
#include "ZIPReaderImpl.h"


ZIPReader::ZIPReader(const std::string& zip_path, const std::string& output_path)
	:
	pimpl(std::make_unique<ZIPReaderImpl>(zip_path, output_path))
{
}

ZIPReader::~ZIPReader()
{

}

ZIPErrorCode ZIPReader::UnZip()
{
	return pimpl->UnZip();
}
