#include "pch.h"
#include "ZIPDeCompressor.h"

#include <zlib.h>

ZIPDeCompressor::ZIPDeCompressor(CompressorMode mode)
	:
	m_mode(mode)
{

}

ZIPErrorCode ZIPDeCompressor::Decompress(std::vector<unsigned char>& compressed, std::vector<unsigned char>& uncompressed)
{
	if (m_mode == CompressorMode::Deflate)
	{
		z_stream strm{};
		strm.next_in = compressed.data();
		strm.avail_in = compressed.size();
		strm.next_out = uncompressed.data();
		strm.avail_out = uncompressed.size();

		inflateInit2(&strm, -MAX_WBITS); // raw deflate
		int ret = inflate(&strm, Z_FINISH);
		inflateEnd(&strm);

		if (ret != Z_STREAM_END) {
			return ZIPErrorCode::DecompressionFailed;
		}
	}
	else
	{
		return ZIPErrorCode::NotSupportedCompressionMode;
	}

	return ZIPErrorCode::Success;
}
