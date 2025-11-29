#include "pch.h"
#include "ZIPReaderImpl.h"
#include "../include/ZIPErrorCode.h"
#include "ZIPStructure.h"
#include "ZIPDeCompressor.h"
#include "ZIPUtils.h"

#include <filesystem>
#include <memory>
#include <iostream>
#include <thread>


namespace fs = std::filesystem;

thread_local int ZIPLastError = 0;  // It will store last error on each thread, usually, it contains OS API error code

ZIPReaderImpl::ZIPReaderImpl(const std::string& zip_path, const std::string& output_path)
	:
	m_zip_path(zip_path),
	m_output_path(output_path),
	m_file_mgr(output_path)
{

}

ZIPErrorCode ZIPReaderImpl::UnZip()
{
	if (ZIPErrorCode ret = ValidateParams(); ret != ZIPErrorCode::Success)
	{
		return ret;
	}

	ReadZipFile();

	return ZIPErrorCode::Success;
}

int ZIPReaderImpl::GetZIPLastError()
{
	return ZIPLastError;
}

ZIPErrorCode ZIPReaderImpl::ValidateParams() const
{
	std::error_code ec;
	if (!fs::is_regular_file(m_zip_path, ec))
	{
		SetZIPLastError(ec.value());
		return ZIPErrorCode::ZipFileNotExisted;
	}

	if (!fs::exists(m_output_path))
	{
		if (!fs::create_directory(m_output_path, ec))
		{
			SetZIPLastError(ec.value());
			return ZIPErrorCode::CreateDirectoryFailed;
		}
	}
	else if (!fs::is_directory(m_output_path, ec))
	{
		SetZIPLastError(ec.value());
		return ZIPErrorCode::OutputDirInvalid;
	}

	return ZIPErrorCode::Success;
}

ZIPErrorCode ZIPReaderImpl::ReadZipFile()
{
	m_zip_file.open(m_zip_path, std::ios::binary);
	if (!m_zip_file)
	{
		return ZIPErrorCode::ZipFileOpenFailed;
	}

	/*
	Zip file structure :
	|Local File Header|Compressed data|Local File Header|Compressed data|...|CentralDirectoryHeader|End Of Central Directory Header|
	*/

	// To parse a zip file, we should seek to end of file and parse EOCD
	EOCD eocd;
	if (ZIPErrorCode ec = ParseEOCD(eocd); ec != ZIPErrorCode::Success)
	{
		return ec;
	}

	auto [ec, file_entries] = ParseCentralDirectory(eocd.offsetOfCD, eocd.totalEntries);
	if (ec != ZIPErrorCode::Success)
	{
		return ec;
	}

	for (const FileEntryInfo& file_entry : file_entries)
	{
		ParseLocalFileHeader(file_entry);
	}

	return ZIPErrorCode::Success;
}

ZIPErrorCode ZIPReaderImpl::ParseEOCD(EOCD& eocd)
{
	/*
The EOCD record has a fixed signature and fields:

Field	Size (bytes)	Description
Signature	4	Always 0x06054b50
Disk number	2	Current disk number
Disk with CD	2	Disk number where central directory starts
Entries on disk	2	Number of entries on this disk
Total entries	2	Total number of entries in central directory
Size of CD	4	Size of central directory (bytes)
Offset of CD	4	Offset of central directory from start of file
Comment length	2	Length of archive comment
Comment	variable	Optional comment
	*/

	// Seek to end
	m_zip_file.seekg(0, std::ios::end);
	size_t file_size = m_zip_file.tellg();

	// EOCD max size will be 22 bytes + max 64KB comments = 65558 bytes
	size_t search_size = min(file_size, 65558);
	m_zip_file.seekg(file_size - search_size, std::ios::beg);

	unsigned char* buffer = new unsigned char[search_size];
	std::unique_ptr<unsigned char[]> auto_release(buffer);  // Auto-release later
	std::memset(buffer, 0, search_size);

	m_zip_file.read(reinterpret_cast<char*>(buffer), search_size);
	for (int i = search_size - 4; i >= 0; --i)
	{
		// Check little endian match 0x06054b50
		if (buffer[i] == 0x50 && buffer[i + 1] == 0x4b && buffer[i + 2] == 0x05 && buffer[i + 3] == 0x06)
		{
			// Found signature byte
			std::memcpy(&eocd, buffer + i, sizeof(EOCD));
			return ZIPErrorCode::Success;
		}
	}

	return ZIPErrorCode::EOCDNotFound;
}

std::tuple<ZIPErrorCode, std::vector<FileEntryInfo>> ZIPReaderImpl::ParseCentralDirectory(uint32_t offset_cd, uint16_t total_entries)
{
	/*
	* Each file in the ZIP has a Central Directory File Header. This is what you parse after locating the Central Directory offset from the EOCD.

Field	Size (bytes)	Description
Signature	4	Always 0x02014b50
Version made by	2	ZIP spec version
Version needed	2	Minimum version to extract
General purpose flag	2	Compression/encryption flags
Compression method	2	e.g., 0 = store, 8 = deflate
Last mod file time	2	DOS time format
Last mod file date	2	DOS date format
CRC-32	4	Checksum of uncompressed data
Compressed size	4	Size of compressed data
Uncompressed size	4	Size of original data
File name length	2	Length of file name
Extra field length	2	Length of extra field
File comment length	2	Length of comment
Disk number start	2	Disk number (multi-disk ZIPs)
Internal file attributes	2	File attributes
External file attributes	4	File attributes
Relative offset of local header	4	Offset to local header
File name	variable	Actual file name
Extra field	variable	Extra metadata
File comment	variable	Optional comment
	*/

	std::vector<FileEntryInfo> file_entries;

	m_zip_file.seekg(offset_cd, std::ios::beg);
	for (int i = 0; i < total_entries; ++i)
	{
		CentralDirectoryHeader header;
		m_zip_file.read(reinterpret_cast<char*>(&header), sizeof(CentralDirectoryHeader));

		if (header.signature != 0x02014b50)
		{
			return { ZIPErrorCode::InvalidCentralDirectorySignature, {} };
		}

		// Read file name
		std::string file_name(header.fileNameLength, '\0');
		m_zip_file.read(&file_name[0], header.fileNameLength);

		file_entries.push_back(FileEntryInfo(header.localHeaderOffset, file_name));

		// Skip extra + comment
		m_zip_file.seekg(header.extraFieldLength + header.fileCommentLength, std::ios::cur);
	}

	return { ZIPErrorCode::Success, file_entries };
}

ZIPErrorCode ZIPReaderImpl::ParseLocalFileHeader(const FileEntryInfo& file_entry)
{
	/*
	* The Local File Header structure:

Field	Size (bytes)	Description
Signature	4	Always 0x04034b50
Version needed	2	Minimum version to extract
Flags	2	Compression/encryption flags
Compression method	2	0 = stored, 8 = deflate
Mod time/date	4	DOS format
CRC-32	4	Checksum
Compressed size	4	Size of compressed data
Uncompressed size	4	Size of original data
File name length	2	Length of file name
Extra field length	2	Length of extra field
File name	variable	Actual file name
Extra field	variable	Extra metadata
After this header, the compressed file data begins.
	*/

	m_zip_file.seekg(file_entry.localHeaderOffset, std::ios::beg);

	LocalFileHeader header;
	m_zip_file.read(reinterpret_cast<char*>(&header), sizeof(header));

	if (header.signature != 0x04034b50) {
		return ZIPErrorCode::InvalidLocalFileHeaderSignature;
	}

	m_zip_file.seekg(header.fileNameLength + header.extraFieldLength, std::ios::cur);

	// Read compressed data
	std::vector<unsigned char> compressed(header.compressedSize);
	m_zip_file.read(reinterpret_cast<char*>(compressed.data()), header.compressedSize);

	std::vector<unsigned char> uncompressed(header.uncompressedSize);

	if (header.compression == 0)
	{
		// Stored mode -> No compression
		uncompressed = compressed;
	}
	else if (header.compression == 8)
	{
		// Deflate compression

		if (compressed.empty())  // There is case compressed size is zero
		{
			std::cout << "[Warn] Compressed size is 0" << std::endl;
		}

		ZIPDeCompressor compressor(CompressorMode::Deflate);
		ZIPErrorCode ec = compressor.Decompress(compressed, uncompressed);
		if (ec != ZIPErrorCode::Success)
		{
			return ec;
		}
	}
	else
	{
		// Not suppored
		std::cerr << "Not supported compress mode: " << header.compression << std::endl;
		return ZIPErrorCode::NotSupportedCompressionMode;
	}

	// Write to output file
	zip_utils::ZipDateTime dt = zip_utils::DecodeDosDateTime(header.modDate, header.modTime);
	if (file_entry.fileName.ends_with("/"))
	{
		// Directory
		auto ec = m_file_mgr.CreateDir(file_entry.fileName);
		if (ec != ZIPErrorCode::Success)
		{
			return ec;
		}
	}
	else
	{
		// File
		m_file_mgr.CreateFile(file_entry.fileName, uncompressed);
	}

	m_file_mgr.SetModifiedTime(file_entry.fileName, dt);

	return ZIPErrorCode::Success;
}

void ZIPReaderImpl::SetZIPLastError(int ec) const
{
	ZIPLastError = ec;
}
