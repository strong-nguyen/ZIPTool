#include "pch.h"
#include "FileManager.h"

#include <fstream>


FileManager::FileManager(const std::string& output_dir)
	:
	m_output_dir(output_dir)
{
}

ZIPErrorCode FileManager::CreateDir(const std::string& dir_name)
{
	std::error_code ec;
	fs::path dir_path = m_output_dir / dir_name;
	fs::create_directories(dir_path, ec);
	if (ec.value() != 0)
	{
		return ZIPErrorCode::CreateDirectoryFailed;
	}

	return ZIPErrorCode::Success;
}

void FileManager::CreateFile(const std::string& file_name, std::vector<unsigned char>& uncompressed)
{
	fs::path file_path = m_output_dir / file_name;
	std::ofstream out(file_path, std::ios::binary);
	out.write(reinterpret_cast<char*>(uncompressed.data()), uncompressed.size());
}
