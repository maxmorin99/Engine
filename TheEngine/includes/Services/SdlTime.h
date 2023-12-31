#pragma once

#include <SDL_stdinc.h>
#include "Interfaces/ITime.h"

namespace Core
{
	class SdlTimer final : public ITimer
	{
	public:
		SdlTimer() = default;

		/** Time Interface */
		virtual ~SdlTimer() override = default;
		virtual void StartTimer() override;
		virtual void UpdateStart() override;
		virtual void UpdateEnd() override;
		virtual void SetTargetFps(const float Fps) override;
		virtual float GetDeltaTime() const override;
		/** End Interface */

	private:
		Uint32 mTargetFps = 0;
		Uint32 mDeltaTime = 0;
		Uint32 mFrameStartTime = 0;
		Uint32 mFrameEndTime = 0;
	};
}
