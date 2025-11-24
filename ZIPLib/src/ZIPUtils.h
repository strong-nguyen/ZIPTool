#pragma once

namespace zip_utils
{
	struct ZipDateTime {
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
	};

	ZipDateTime DecodeDosDateTime(uint16_t dosDate, uint16_t dosTime);
}
