#pragma once

#include <cstdint>

#pragma pack(push, 1)
// Ensure memory layout is kept
struct EOCD {
	uint32_t signature;
	uint16_t diskNumber;
	uint16_t diskWithCD;
	uint16_t entriesOnDisk;
	uint16_t totalEntries;
	uint32_t sizeOfCD;
	uint32_t offsetOfCD;
	uint16_t commentLength;

	EOCD();
};
#pragma pack(pop)

#pragma pack(push, 1)
// Ensure memory layout is kept
struct CentralDirectoryHeader {
	uint32_t signature;
	uint16_t versionMadeBy;
	uint16_t versionNeeded;
	uint16_t flags;
	uint16_t compression;
	uint16_t modTime;
	uint16_t modDate;
	uint32_t crc32;
	uint32_t compressedSize;
	uint32_t uncompressedSize;
	uint16_t fileNameLength;
	uint16_t extraFieldLength;
	uint16_t fileCommentLength;
	uint16_t diskNumberStart;
	uint16_t internalAttr;
	uint32_t externalAttr;
	uint32_t localHeaderOffset;

	CentralDirectoryHeader();
};
#pragma pack(pop)

#pragma pack(push, 1)
struct LocalFileHeader {
	uint32_t signature;
	uint16_t version;
	uint16_t flags;
	uint16_t compression;
	uint16_t modTime;
	uint16_t modDate;
	uint32_t crc32;
	uint32_t compressedSize;
	uint32_t uncompressedSize;
	uint16_t fileNameLength;
	uint16_t extraFieldLength;

	LocalFileHeader();
};
#pragma pack(pop)


// After parsing Central Directory header, we receive a list of FileEntryInfo
class FileEntryInfo
{
public:
	FileEntryInfo(uint32_t localHeaderOffset_, const std::string& fileName_);

	uint32_t localHeaderOffset = 0;

	std::string fileName;
};