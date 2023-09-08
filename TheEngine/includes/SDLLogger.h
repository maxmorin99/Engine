#pragma once

#include "ILogger.h"

namespace Core
{
	class SdlLogger : public ILogger
	{
	public:
		// H�rit� via ILogger
		SdlLogger();
		~SdlLogger();
		void DebugLog(EColor TextColor, const char* Message, ...) override;
	};
}