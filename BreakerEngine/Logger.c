#include "Logger.h"
#include "FileSystem.h"
#include "stdString.h"

#include <Windows.h>
#include <stdio.h>
#include "Tools.h"

#undef ERROR

fsBool log_file_is_created = FALSE;

void SetupLogFile()
{
#if !LOG_ACTIVE
	return;
#endif

	Path file_path = fs_create_path(LOG_FILE_PATH);
	Path directory_path = file_path.parent(&file_path);

	if (!directory_path.exist(&directory_path))
	{
		fs_create_directory(directory_path.path(&directory_path));
	}


	if (exist(&file_path))
	{
		stdString* old_file_new_name = stdStringCreate(directory_path.path(&directory_path));
		old_file_new_name->append(old_file_new_name, "/");
		Path clean_name = file_path.stem(&file_path);
		Path extension = file_path.extension(&file_path);
		old_file_new_name->append(old_file_new_name, clean_name.path(&clean_name));
		old_file_new_name->append(old_file_new_name, IntToString(rand() % 10000000));
		old_file_new_name->append(old_file_new_name, ".");
		old_file_new_name->append(old_file_new_name, extension.path(&extension));
		(void)rename(file_path.path(&file_path), old_file_new_name->getData(old_file_new_name));

		old_file_new_name->destroy(&old_file_new_name);
	}

	FILE* file = fopen(file_path.path(&file_path), "w");
	if (!file)
	{
		fprintf(stderr, "Error while creating log file\nTerminating the executable.");
		exit(-1);
	}
	fclose(file);
	log_file_is_created = TRUE;
}

static void PutLogToFile(const char* Message)
{
	if (!MainThreadHasStarted)
	{
		return;
	}

	if (!log_file_is_created)
	{
		fprintf(stderr, "Log file was not created, did you call SetupLogFile ?\nTerminating the executable.");
	}

	FILE* file = fopen(LOG_FILE_PATH, "a");
	if (!file)
	{
		fprintf(stderr, "Error while opening the log file\nTerminating the executable.");
		exit(-1);
	}

	fprintf(file, Message);

	fclose(file);
}


void Log(CategoryHelper Category, Verbosity verbosity, const char* format, ...)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char formatted_message[LOG_MAX_LENGTH];
	char log_message[LOG_MAX_LENGTH];


	va_list args;
	va_start(args, format);
	(void)vsprintf_s(formatted_message, LOG_MAX_LENGTH, format, args);
	va_end(args);


	switch (verbosity) {
	case MESSAGE:
		(void)sprintf_s(log_message, LOG_MAX_LENGTH, "%s: %s: %s\n", Category.Category_Name, "Message", formatted_message);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("%s", log_message);
		break;
	case WARNING:
		(void)sprintf_s(log_message, LOG_MAX_LENGTH, "%s: %s: %s\n", Category.Category_Name, "Warning", formatted_message);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		printf("%s", log_message);
		break;
	case ERROR:
		(void)sprintf_s(log_message, LOG_MAX_LENGTH, "%s: %s: %s\n", Category.Category_Name, "Error", formatted_message);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		fprintf(stderr, "%s", log_message);
		break;
	}

	PutLogToFile(log_message);

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}