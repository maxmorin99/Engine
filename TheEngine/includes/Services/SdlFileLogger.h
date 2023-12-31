#pragma once

#include "Interfaces/ILogger.h"

namespace Core
{
	class SdlFileLogger final : public ILogger
	{
	public:
		// Logger interface
		SdlFileLogger() = default;
		virtual ~SdlFileLogger() override = default;
		virtual bool Init() override;
		virtual void DebugLog(const ConsoleColor& TextColor, const char* Message, ...) override;
		virtual void Shutdown() override;

	private:
		void* mFile = nullptr;
	};
}