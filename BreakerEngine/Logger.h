#pragma once

#define LOG_MAX_LENGTH 512
#define LOG_ACTIVE 0

#define LOG_FILE_PATH "../Logs/LogReport.log" 

typedef struct CategoryHelper { const char* Category_Name; } CategoryHelper;

#if LOG_ACTIVE
#define DEFINE_LOG_CATEGORY(Category) typedef CategoryHelper Category;
#define LOG(Category, Verbosity, Format, ...) Log((CategoryHelper){.Category_Name = #Category}, ##Verbosity, Format, __VA_ARGS__)
#else
#define LOG(Category, Verbosity, Format, ...)
#define DEFINE_LOG_CATEGORY(Category)
#endif

enum Verbosity
{
	MESSAGE,
	WARNING,
	ERROR
};
typedef enum Verbosity Verbosity;

void SetupLogFile();
void Log(CategoryHelper Category, Verbosity verbosity, const char* format, ...);

#undef ERROR