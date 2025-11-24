#pragma once

#include "../include/ZIPErrorCode.h"
#include "ZIPUtils.h"

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FileManager
{
public:
	FileManager(const std::string& output_dir);

	ZIPErrorCode CreateDir(const std::string& dir_name);

	void CreateFile(const std::string& file_name, std::vector<unsigned char>& uncompressed);

	bool SetModifiedTime(const std::string& file_name, const zip_utils::ZipDateTime& dt);

private:
	fs::path m_output_dir;
};

