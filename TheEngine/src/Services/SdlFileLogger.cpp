#include "Services/SdlFileLogger.h"
#include <iostream>
#include <fstream>
#include <cstdarg>

bool Core::SdlFileLogger::Init()
{
	// Open the file
	mFile = new std::ofstream("DebugLog.txt");
	if (mFile == nullptr) return false;
	return true;
}

void Core::SdlFileLogger::DebugLog(const ConsoleColor& TextColor, const char* Message, ...)
{
	if (!mFile) return;
	auto* MyFile = static_cast<std::ofstream*>(mFile);
	if (!MyFile) return;
	va_list args;
	va_start(args, Message);
	char Buffer[256];
	const int bFormating = vsnprintf(Buffer, sizeof(Buffer), Message, args);
	if (!bFormating) return;
	*MyFile << Buffer << std::endl;
	va_end(args);
}

void Core::SdlFileLogger::Shutdown()
{
	// Close the file
	if (!mFile) return;
	auto* MyFile = static_cast<std::ofstream*>(mFile);
	if (!MyFile) return;
	MyFile->close();
	mFile = nullptr;
}