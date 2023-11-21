#pragma once

#include "AtlasComponent.h"
#include "Interfaces/IUpdatable.h"

namespace Core
{
	struct Clip
	{
		Clip() = default;
		Clip(const std::string& InName, std::vector<Frame>& InFrames, float InFrameTime = 0.f);
		void Reset();
		void AddFrame(Frame& InFrame);

		std::string Name;

		/** List of frames that compose the clip animation */
		std::vector<Frame> Frames;

		/** Time between frame changes */
		float FrameTime = 0.f;

		/** Current time elapsed since last frame and the current frame */
		float CurrFrameTime = 0.f;

		/** Current index of the current frame in the frame list */
		int CurrFrameIndex = 0;

		bool Loop = false;

		/** if loop is set to false and EndCallback != nullptr, EndCallback will be called when the sprite sequence is done playing */
		std::function<void()> EndCallback;
	};

	class AnimationComponent : public AtlasComponent, public IUpdatable
	{
	public:
		AnimationComponent(Object* Owner);
		virtual ~AnimationComponent() = default;
		virtual void Start();
		virtual void Destroy();

		
		void Update(float DeltaTime) override;
		virtual void Draw() override;

		void AddClip(const std::string& InName, Clip& InClip);
		void SetClip(const std::string& InClipName, bool bLoop, std::function<void()> FunPtr = nullptr);
		void SetDefaultClip(Clip& InClip);

	private:
		std::unordered_map<std::string, Clip> mClips;
		Clip mCurrentClip;

		/** Default clip. This clip will be selected when a non looping animation ends and no other is requested */
		Clip mDefaultClip;

	public:
		inline std::string GetCurrentClipName() const { return mCurrentClip.Name; }
	};
}