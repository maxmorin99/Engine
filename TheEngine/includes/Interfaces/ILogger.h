#pragma once

#include "Utility.h"

namespace Core
{
	class ILogger
	{
	public:
		virtual ~ILogger() = default;

		/**
		* @brief Initializes the logger.
		*/
		virtual bool Init() = 0;

		/**
		* @brief Logs a debug message to the console with specified text color.
		* @param TextColor The color to use for the text in the log message.
		* @param Message The format string of the log message.
		* @param ... Additional arguments for the format string.
		*/
		virtual void DebugLog(const ConsoleColor& TextColor, const char* Message, ...) = 0;

		/**
		* @brief Shuts down the logger, releasing any allocated resources.
		*/
		virtual void Shutdown() = 0;
	};
}