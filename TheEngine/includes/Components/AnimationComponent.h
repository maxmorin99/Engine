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
		virtual Component* Clone(Object* Owner) override;
		virtual void SetupClone(Component* Child) override;
		
		void Update(float DeltaTime) override;
		virtual void Draw() override;

		/** Add a new animation clip
		*	@param InName: Name of the clip to add
		*	@param InClip: Reference to the clip to add
		*/
		void AddClip(const std::string& InName, Clip& InClip);

		/** Set a new clip to start playing it
		*	@param InClipName: Name of the clip
		*	@param bLoop: Loop or not the clip
		*	@param FunPtr: Function to call when a non looping clip ends
		*/
		void SetClip(const std::string& InClipName, bool bLoop, std::function<void()> FunPtr = nullptr);

		/** This clip will be selected when a non looping animation ends and no other is requested
		*	@param InClip: Reference to the clip that will be considered as the default clip
		*/
		void SetDefaultClip(Clip& InClip);

		/** Allow this component to update animation */
		void Enable();

		/** Stop this component to update animation */
		void Disable();

	private:
		std::unordered_map<std::string, Clip> mClips;
		Clip mCurrentClip;

		/** Default clip. This clip will be selected when a non looping animation ends and no other is requested */
		Clip mDefaultClip;

		Color mDeathAnimationColor = Color::White;

		bool bShouldPlay = true;

	public:
		/** Get the name of the currently playing clip */
		inline std::string GetCurrentClipName() const { return mCurrentClip.Name; }

		/** Set the color for the death animation */
		inline void SetDeathAnimationColor(const Color& InColor) { mDeathAnimationColor = InColor; }

		/** Get the color set for the death animation */
		inline Color GetDeathAnimationColor() const { return mDeathAnimationColor; }
	};
}