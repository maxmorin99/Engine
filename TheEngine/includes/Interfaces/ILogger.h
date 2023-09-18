#pragma once

#include "Utility.h"

namespace Core
{
	class ILogger
	{
	public:
		virtual ~ILogger() = default;
		virtual bool Init() = 0;
		virtual void DebugLog(const ConsoleColor& TextColor, const char* Message, ...) = 0;
		virtual void Shutdown() = 0;
	};
}