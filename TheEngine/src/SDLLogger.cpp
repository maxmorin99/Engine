#include "SDLLogger.h"
#include "Windows.h"
#include <cstdio>
#include <stdarg.h>


Core::SdlLogger::SdlLogger() : ILogger()
{
	AllocConsole();
	int _r = freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
}

Core::SdlLogger::~SdlLogger()
{
	FreeConsole();
}

void Core::SdlLogger::DebugLog(EColor TextColor, const char* Message, ...)
{
	const UINT8 NewColor = static_cast<UINT8>(TextColor);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, NewColor);
	
	/** 
	* Print the message and all arguments to the console. Ref: https://walidezzat.hashnode.dev/printf-like-function
	*/
	va_list args;
	va_start(args, Message);
	vprintf(Message, args);
	va_end(args);
}