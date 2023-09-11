#include "SdlTime.h"
#include <SDL_timer.h>

void Core::SdlTimer::StartTimer()
{
	_FrameEndTime = SDL_GetTicks();
}

void Core::SdlTimer::UpdateStart()
{
	_FrameStartTime = SDL_GetTicks();
	_DeltaTime = _FrameStartTime - _FrameEndTime;
}

void Core::SdlTimer::UpdateEnd()
{
	if(_DeltaTime < _TargetFps)
	{
		const Uint32 SleepTime = _TargetFps - _DeltaTime;
		SDL_Delay(SleepTime);
	}
	_FrameEndTime = _FrameStartTime;
}

void Core::SdlTimer::SetTargetFps(const float Fps)
{
	_TargetFps = 1000 / (Uint32)Fps;
}

float Core::SdlTimer::GetDeltaTime() const
{
	return (float)_DeltaTime / 1000;
}
