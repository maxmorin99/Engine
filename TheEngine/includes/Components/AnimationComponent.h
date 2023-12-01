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

		/** Called to clone this component and all important member variables */
		virtual Component* Clone(Object* Owner) override;

		/** If a child component calls Clone(), it will call SetupClone to clone all important member variables of its parents component */
		virtual void SetupClone(Component* Child) override;
		
		void Update(float DeltaTime) override;
		virtual void Draw() override;

		/** Add a new animation clip */
		void AddClip(const std::string& InName, Clip& InClip);

		/** Set a new clip to start playing it */
		void SetClip(const std::string& InClipName, bool bLoop, std::function<void()> FunPtr = nullptr);

		/** This clip will be selected when a non looping animation ends and no other is requested */
		void SetDefaultClip(Clip& InClip);
		void Enable();
		void Disable();

	private:
		std::unordered_map<std::string, Clip> mClips;
		Clip mCurrentClip;

		/** Default clip. This clip will be selected when a non looping animation ends and no other is requested */
		Clip mDefaultClip;

		Color mDeathAnimationColor = Color::White;

		bool bShouldPlay = true;

	public:
		inline std::string GetCurrentClipName() const { return mCurrentClip.Name; }
		inline void SetDeathAnimationColor(const Color& InColor) { mDeathAnimationColor = InColor; }
		inline Color GetDeathAnimationColor() const { return mDeathAnimationColor; }
	};
}