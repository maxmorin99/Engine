#include "SdlFileLogger.h"
#include <iostream>
#include <fstream>
#include <cstdarg>

bool Core::SdlFileLogger::Init()
{
	// Open the file
	_File = new std::ofstream("DebugLog.txt");
	if (_File == nullptr) return false;
	return true;
}

void Core::SdlFileLogger::DebugLog(EColor TextColor, const char* Message, ...)
{
	if (!_File) return;
	auto* MyFile = static_cast<std::ofstream*>(_File);
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
	if (!_File) return;
	auto* MyFile = static_cast<std::ofstream*>(_File);
	if (!MyFile) return;
	MyFile->close();
	_File = nullptr;
}