#include "CrashHandler.h"
#include "Logger.h"


#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

#define MAX_STACK_FRAMES 64
#define MAX_SYMBOL_LENGTH 256

static void GetCrashCallStack(CONTEXT* contextRecord, char* outBuffer, size_t bufferSize)
{
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    SymInitialize(process, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME64));
    DWORD imageType;

#ifdef _M_X64
    imageType = IMAGE_FILE_MACHINE_AMD64;
    stackFrame.AddrPC.Offset = contextRecord->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = contextRecord->Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = contextRecord->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#else
    imageType = IMAGE_FILE_MACHINE_I386;
    stackFrame.AddrPC.Offset = contextRecord->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = contextRecord->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = contextRecord->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

    outBuffer[0] = '\0';
    size_t currentOffset = 0;

    for (int frameNum = 0; frameNum < MAX_STACK_FRAMES; ++frameNum)
    {
        BOOL success = StackWalk64(
            imageType, process, thread, &stackFrame, contextRecord,
            NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL
        );

        if (!success || stackFrame.AddrPC.Offset == 0) {
            break;
        }

        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYMBOL_LENGTH * sizeof(char)];
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbolBuffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYMBOL_LENGTH;

        DWORD64 displacement = 0;
        char lineStr[512] = "";

        if (SymFromAddr(process, stackFrame.AddrPC.Offset, &displacement, symbol)) {
            IMAGEHLP_LINE64 line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            DWORD lineDisplacement = 0;

            if (SymGetLineFromAddr64(process, stackFrame.AddrPC.Offset, &lineDisplacement, &line)) {
                snprintf(lineStr, sizeof(lineStr), "%s() [File: %s, Line: %lu]",
                    symbol->Name, line.FileName, line.LineNumber);
            }
            else {
                snprintf(lineStr, sizeof(lineStr), "%s() [Address: 0x%llX]",
                    symbol->Name, symbol->Address);
            }
        }
        else {
            snprintf(lineStr, sizeof(lineStr), "Unknown Function [Address: 0x%llX]",
                stackFrame.AddrPC.Offset);
        }

        int written = snprintf(outBuffer + currentOffset, bufferSize - currentOffset,
            "[%02d] %s\n", frameNum, lineStr);

        if (written > 0) {
            currentOffset += written;
            if (currentOffset >= bufferSize) break;
        }
    }

    SymCleanup(process);
}


static LONG WINAPI CustomExceptionHandler(EXCEPTION_POINTERS* exceptionInfo)
{
    fprintf(stderr, "\n\nFatal error: Unhandled exception caught!\n");

    DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;
    fprintf(stderr, "Exception Code: 0x%lX\n\n", exceptionCode);
     
    char callStackString[8192];
    GetCrashCallStack(exceptionInfo->ContextRecord, callStackString, sizeof(callStackString));
     
    fprintf(stderr, "-------------------------Crash Report-------------------------\n%s\n-------------------------End Crash Report---------------------\n", callStackString);
     
    const char* logFileName = LOG_FILE_PATH;
    HANDLE hFile = CreateFileA(
        logFileName,
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
     

    if (hFile != INVALID_HANDLE_VALUE)
    {
        SetFilePointer(hFile, 0, NULL, FILE_END);

        char headerBuffer[256];
        int headerLen = snprintf(headerBuffer, sizeof(headerBuffer),
            "\n\n-------------------------Crash Report-------------------------\nException Code: 0x%lX\n\n--- CALL STACK ---\n",
            exceptionCode);

        DWORD bytesWritten = 0;
         
        WriteFile(hFile, headerBuffer, headerLen, &bytesWritten, NULL);
        WriteFile(hFile, callStackString, (DWORD)strlen(callStackString), &bytesWritten, NULL);
        WriteFile(hFile, "\n-------------------------End Crash Report---------------------\n", 65, &bytesWritten, NULL);

        CloseHandle(hFile); 
    }
    else
    {
        fprintf(stderr, "\nFailed to create crash log file on disk.\n");
    }

    return EXCEPTION_EXECUTE_HANDLER;
}


void SetCustomExceptionHandler()
{
#if LOG_ACTIVE
    SetUnhandledExceptionFilter(CustomExceptionHandler);
#endif
}