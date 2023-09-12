#pragma once

#include "ILogger.h"

namespace Core
{
	class SdlConsoleLogger final : public ILogger
	{
	public:
		/** Logger Interface */
		SdlConsoleLogger() = default;
		virtual ~SdlConsoleLogger() override = default;
		virtual bool Init() override;
		virtual void Shutdown() override;
		void DebugLog(EColor TextColor, const char* Message, ...) override;
		/** End Interface */
	};
}