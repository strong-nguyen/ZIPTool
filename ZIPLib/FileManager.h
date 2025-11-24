#pragma once

#include "include/ZIPErrorCode.h"

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

private:
	fs::path m_output_dir;
};

