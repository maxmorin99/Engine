#pragma once

namespace Core
{
	enum class EColor
	{
		Black = 0,
		Blue = 1,
		Green = 2,
		Cyan = 3,
		Red = 4,
		Purple = 5,
		Yellow = 6,
		White = 7,
		Grey = 8,

		MAX
	};

	class ILogger
	{
	public:
		virtual ~ILogger() = default;
		virtual void DebugLog(EColor TextColor, const char* Message, ...) = 0;
	};
}