#include "pch.h"
#include "ZIPUtils.h"

namespace zip_utils
{
	ZipDateTime DecodeDosDateTime(uint16_t dosDate, uint16_t dosTime)
	{
		/*
		* DOS Date/Time Encoding Recap
Time (16 bits):

Bits 0–4 → seconds ÷ 2 (0–29 → 0–58 seconds)

Bits 5–10 → minutes (0–59)

Bits 11–15 → hours (0–23)

Date (16 bits):

Bits 0–4 → day (1–31)

Bits 5–8 → month (1–12)

Bits 9–15 → years since 1980 (0–127 → 1980–2107)
		*/
		ZipDateTime dt;

		dt.second = (dosTime & 0x1F) * 2;          // bits 0-4
		dt.minute = (dosTime >> 5) & 0x3F;         // bits 5-10
		dt.hour = (dosTime >> 11) & 0x1F;        // bits 11-15

		dt.day = dosDate & 0x1F;                // bits 0-4
		dt.month = (dosDate >> 5) & 0x0F;         // bits 5-8
		dt.year = ((dosDate >> 9) & 0x7F) + 1980;// bits 9-15

		return dt;
	}
}