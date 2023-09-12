#pragma once

#include "ILogger.h"

namespace Core
{
	class SdlFileLogger final : public ILogger
	{
	public:
		// Logger interface
		SdlFileLogger() = default;
		virtual ~SdlFileLogger() override = default;
		virtual bool Init() override;
		virtual void DebugLog(EColor TextColor, const char* Message, ...) override;
		virtual void Shutdown() override;

	private:
		void* _File = nullptr;
	};
}