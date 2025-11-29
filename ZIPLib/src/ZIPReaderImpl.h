#pragma once

#include "../include/ZIPErrorCode.h"
#include "ZIPStructure.h"
#include "FileManager.h"

#include <fstream>
#include <tuple>
#include <vector>



class ZIPReaderImpl
{
public:
	ZIPReaderImpl(const std::string& zip_path, const std::string& output_path);

	ZIPErrorCode UnZip();

	int GetZIPLastError();

private:
	ZIPErrorCode ValidateParams() const;

	ZIPErrorCode ReadZipFile();

	ZIPErrorCode ParseEOCD(EOCD& eocd);

	std::tuple<ZIPErrorCode, std::vector<FileEntryInfo>> ParseCentralDirectory(uint32_t offset_cd, uint16_t total_entries);

	ZIPErrorCode ParseLocalFileHeader(const FileEntryInfo& file_entry);

	void SetZIPLastError(int ec) const;

	std::string m_zip_path;

	std::ifstream m_zip_file;

	std::string m_output_path;

	FileManager m_file_mgr;
};

