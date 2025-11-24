#pragma once

#include "../include/ZIPErrorCode.h"
#include <vector>


enum class CompressorMode
{
	Deflate
};

class ZIPDeCompressor
{
public:
	ZIPDeCompressor(CompressorMode mode);

	ZIPErrorCode Decompress(std::vector<unsigned char>& compressed, std::vector<unsigned char>& uncompressed);
private:
	CompressorMode m_mode;
};

