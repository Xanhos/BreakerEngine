#pragma once
#include "Tools.h" 
#undef ERROR

#ifdef _DEBUG
#define LOG(Category, Verbosity, Format, ...) Log(Category, ##Verbosity, Format, __VA_ARGS__)
#else
#define LOG(Category, Verbosity, Format, ...)
#endif

enum Verbosity
{
	MESSAGE,
	WARNING,
	ERROR
};
typedef enum Verbosity Verbosity;

void Log(const char* Category, Verbosity verbosity, const char* format, ...);