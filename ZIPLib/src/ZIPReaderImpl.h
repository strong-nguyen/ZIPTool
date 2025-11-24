#pragma once

#include "../include/ZIPErrorCode.h"
#include "../ZIPStructure.h"

#include <fstream>
#include <tuple>
#include <vector>



class ZIPReaderImpl
{
public:
	ZIPReaderImpl(const std::string& zip_path, const std::string& output_path);

	ZIPErrorCode UnZip();

private:
	ZIPErrorCode ValidateParams() const;

	ZIPErrorCode ReadZipFile();

	ZIPErrorCode ParseEOCD(EOCD& eocd);

	std::tuple<ZIPErrorCode, std::vector<FileEntryInfo>> ParseCentralDirectory(uint32_t offset_cd, uint16_t total_entries);

	ZIPErrorCode ParseLocalFileHeader(const FileEntryInfo& file_entry);

	std::string m_zip_path;

	std::string m_output_path;

	std::ifstream m_zip_file;
};

