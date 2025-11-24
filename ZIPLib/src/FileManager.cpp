#include "pch.h"
#include "FileManager.h"

#include <fstream>
#include <chrono>
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <format>


FileManager::FileManager(const std::string& output_dir)
	:
	m_output_dir(output_dir)
{
}

bool FileManager::SetModifiedTime(const std::string& file_name, const zip_utils::ZipDateTime& dt)
{
	// Open file (or directory) with write attributes permission
	DWORD flags = 0;
	if (file_name.ends_with("/"))
	{
		// To open dir
		flags = FILE_FLAG_BACKUP_SEMANTICS;
	}
	else
	{
		flags = FILE_ATTRIBUTE_NORMAL;
	}

	std::string file_name_windows = file_name;
	std::replace(file_name_windows.begin(), file_name_windows.end(), '/', '\\');

	fs::path file_path = m_output_dir / file_name_windows;
	HANDLE hFile = ::CreateFileW(
		file_path.wstring().c_str(),
		FILE_WRITE_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		flags,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		std::cerr << std::format("Failed to open file: {}, ec: {}", file_path.string(), ::GetLastError()) << std::endl;
		return false;
	}

	SYSTEMTIME st = {};
	st.wYear = dt.year;
	st.wMonth = dt.month;
	st.wDay = dt.day;
	st.wHour = dt.hour;
	st.wMinute = dt.minute;
	st.wSecond = dt.second;

	// Convert to FILETIME
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);

	// Apply modified time (last write time)
	BOOL ret = ::SetFileTime(hFile, NULL, NULL, &ft);
	CloseHandle(hFile);

	if (!ret)
	{
		std::cerr << std::format("SetFileTime failed for {}, ec: {}", file_path.string(), ::GetLastError()) << std::endl;
		return false;
	}

	return true;
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
