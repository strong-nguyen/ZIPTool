#pragma once

#ifdef ZIPLIB_EXPORTS
#define ZIPLIB_API __declspec(dllexport)
#else
#define ZIPLIB_API __declspec(dllimport)
#endif