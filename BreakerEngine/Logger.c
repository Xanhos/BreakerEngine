#include "Logger.h"
#include <Windows.h>

void Log(const char* Category, Verbosity verbosity, const char* format, ...)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char formatted_message[LOG_MAX_LENGTH];
	char log_message[LOG_MAX_LENGTH];


	va_list args;
	va_start(args, format);
	(void)vsprintf_s(formatted_message, LOG_MAX_LENGTH, format, args);
	va_end(args);

	(void)sprintf_s(log_message, LOG_MAX_LENGTH, "\n%s : %s\n", Category, formatted_message);

	switch (verbosity) {
	case MESSAGE:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case WARNING:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case ERROR:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		break;
	}

	printf("%s", log_message);

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}