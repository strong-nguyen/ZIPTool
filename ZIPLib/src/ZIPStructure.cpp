#include "pch.h"
#include "ZIPStructure.h"


EOCD::EOCD()
{
	std::memset(this, 0, sizeof(EOCD));
}

CentralDirectoryHeader::CentralDirectoryHeader()
{
	std::memset(this, 0, sizeof(CentralDirectoryHeader));
}

LocalFileHeader::LocalFileHeader()
{
	std::memset(this, 0, sizeof(LocalFileHeader));
}

FileEntryInfo::FileEntryInfo(uint32_t localHeaderOffset_, const std::string& fileName_)
	:
	localHeaderOffset(localHeaderOffset_),
	fileName(fileName_)
{

}