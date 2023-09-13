#pragma once

namespace Core
{
	class ITimer
	{
	public:
		virtual ~ITimer() = default;
		virtual void StartTimer() = 0;
		virtual void UpdateStart() = 0;
		virtual void UpdateEnd() = 0;
		virtual void SetTargetFps(const float Fps) = 0;
		virtual float GetDeltaTime() const = 0;
	};
}