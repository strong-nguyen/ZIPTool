#pragma once

#include "../include/ZIPError.h"
#include "../ZIPStructure.h"

#include <fstream>
#include <tuple>
#include <vector>



class ZIPReaderImpl
{
public:
	ZIPReaderImpl(const std::string& zip_path, const std::string& output_path);

	ZIPError UnZip();

private:
	ZIPError ValidateParams() const;

	ZIPError ReadZipFile();

	ZIPError ParseEOCD(EOCD& eocd);

	std::tuple<ZIPError, std::vector<FileEntryInfo>> ParseCentralDirectory(uint32_t offset, uint16_t total_entries);

	ZIPError ParseLocalFileHeader(const FileEntryInfo& file_entry);

	std::string m_zip_path;

	std::string m_output_path;

	std::ifstream m_zip_file;
};

