#include "Components/AnimationComponent.h"
#include "Object.h"
#include <iostream>
#include <sstream>   

// Struct Clip ---------------------------------------------------------- //

Core::Clip::Clip(const std::string& InName, std::vector<Frame>& InFrames, float InFrameTime) :
	Name(InName), FrameTime(InFrameTime)
{
	Frames = InFrames;
	for (Frame& F : Frames)
	{
		F.Rect.X *= F.Rect.W;
		F.Rect.Y *= F.Rect.H;
	}
}

void Core::Clip::Reset()
{
	CurrFrameIndex = 0;
	CurrFrameTime = 0.f;
	EndCallback = nullptr;
}

void Core::Clip::AddFrame(Frame& InFrame)
{
	InFrame.Rect.X *= InFrame.Rect.W;
	InFrame.Rect.Y *= InFrame.Rect.H;
	Frames.push_back(InFrame);
}


// AnimationComponent --------------------------------------------------- //

Core::AnimationComponent::AnimationComponent(Object* Owner):
	AtlasComponent(Owner)
{
}

void Core::AnimationComponent::Start()
{
	AtlasComponent::Start();
}

void Core::AnimationComponent::Update(float DeltaTime)
{
	if (!bShouldPlay && mCurrentClip.Frames.size() == 0) return;

	// Update the current clip
	mCurrentClip.CurrFrameTime += DeltaTime;
	if (mCurrentClip.CurrFrameTime > mCurrentClip.FrameTime)
	{
		mCurrentClip.CurrFrameTime = 0.f;
		mCurrentClip.CurrFrameIndex++;
		if (mCurrentClip.CurrFrameIndex == mCurrentClip.Frames.size())
		{
			if (mCurrentClip.Loop)
			{
				mCurrentClip.CurrFrameIndex = 0;
			}
			else
			{
				if (mCurrentClip.EndCallback)
				{
					mCurrentClip.EndCallback();
					mCurrentClip.Reset();
				}
				
				mCurrentClip = mDefaultClip;
			}
		}
	}

	if (mCurrentClip.Frames.size() > 0)
	{
		SetFrame(mCurrentClip.Frames[mCurrentClip.CurrFrameIndex]);
	}
}

void Core::AnimationComponent::Destroy()
{
	AtlasComponent::Destroy();
}

void Core::AnimationComponent::Enable()
{
	bShouldPlay = true;
}

void Core::AnimationComponent::Disable()
{
	bShouldPlay = false;
}

void Core::AnimationComponent::Draw()
{
	AtlasComponent::Draw();

	if (!mOwner) return;
}

void Core::AnimationComponent::AddClip(const std::string& InName, Clip& InClip)
{
	if (mClips.count(InName) == 0)
	{
		mClips[InName] = InClip;
		InClip.Name = InClip.Name == "" ? InName : InClip.Name;
		
		// not necessary, to remove later
		if (mFrames.count(InName) == 0)
		{
			for (Frame F : InClip.Frames)
			{
				mFrames[F.Name] = F;
			}
		}
	}
}

void Core::AnimationComponent::SetClip(const std::string& InClipName, bool bLoop, std::function<void(void)> EndCallback)
{
	if (mClips.count(InClipName) == 0 || mCurrentClip.Name == InClipName) return;
	
	mCurrentClip.Reset();
	mCurrentClip = mClips[InClipName];
	mCurrentClip.Loop = bLoop;
	mCurrentClip.EndCallback = EndCallback;
}

void Core::AnimationComponent::SetDefaultClip(Clip& InClip)
{
	AddClip("Default", InClip);
	mDefaultClip = InClip;
}