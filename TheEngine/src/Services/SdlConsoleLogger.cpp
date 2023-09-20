#include "Services/SdlConsoleLogger.h"
#include "Windows.h"
#include <cstdio>
#include <cstdarg>

bool Core::SdlConsoleLogger::Init()
{
	AllocConsole();
	int _r = freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
	return !_r;
}

void Core::SdlConsoleLogger::DebugLog(const ConsoleColor& TextColor, const char* Message, ...)
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

void Core::SdlConsoleLogger::Shutdown()
{
	FreeConsole();
}