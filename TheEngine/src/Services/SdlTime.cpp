#include "Services/SdlTime.h"
#include <SDL_timer.h>

void Core::SdlTimer::StartTimer()
{
	mFrameEndTime = SDL_GetTicks();
}

void Core::SdlTimer::UpdateStart()
{
	mFrameStartTime = SDL_GetTicks();
	mDeltaTime = mFrameStartTime - mFrameEndTime;
}

void Core::SdlTimer::UpdateEnd()
{
	Uint32 CurrFrameTime = SDL_GetTicks();
	Uint32 TotalFrameTime = CurrFrameTime - mFrameStartTime;

	if(TotalFrameTime < mTargetFps)
	{
		const Uint32 SleepTime = mTargetFps - TotalFrameTime;
		SDL_Delay(SleepTime);
	}

	mFrameEndTime = mFrameStartTime;
}

void Core::SdlTimer::SetTargetFps(const float Fps)
{
	mTargetFps = 1000 / (Uint32)Fps;
}

float Core::SdlTimer::GetDeltaTime() const
{
	return (float)mDeltaTime / 1000;
}
