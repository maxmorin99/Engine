#pragma once

#include "ILogger.h"

namespace Core
{
	class SdlLogger : public ILogger
	{
	public:
		// Hérité via ILogger
		SdlLogger();
		~SdlLogger();
		void DebugLog(EColor TextColor, const char* Message, ...) override;
	};
}