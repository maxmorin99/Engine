#pragma once

#include "Interfaces/ILogger.h"

namespace Core
{
	class SdlConsoleLogger final : public ILogger
	{
	public:
		/** Logger Interface */
		SdlConsoleLogger() = default;
		virtual ~SdlConsoleLogger() = default;
		virtual bool Init() override;
		virtual void Shutdown() override;
		void DebugLog(const ConsoleColor& TextColor, const char* Message, ...) override;
		/** End Interface */
	};
}