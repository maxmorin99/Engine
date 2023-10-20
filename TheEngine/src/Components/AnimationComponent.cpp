#include "Components/AnimationComponent.h"
#include "Object.h"
#include <iostream>
#include <sstream>   

// Struct Clip ---------------------------------------------------------- //

Core::Clip::Clip(const std::string& InName, std::vector<Frame> InFrames, float InFrameTime) :
	Name(InName), Frames(InFrames), FrameTime(InFrameTime)
{
}

void Core::Clip::Reset()
{
	CurrFrameIndex = 0;
	CurrFrameTime = 0.f;
}

void Core::Clip::AddFrame(const Frame& InFrame)
{
	Frames.push_back(InFrame);
}

void Core::Clip::AddFramesByIndex(std::vector<int> Indexes, int W, int H)
{
	for (int i = 0; i < Indexes.size(); i++)
	{
		std::stringstream Stream;
		Stream << i;
		std::string FrameId;
		Stream >> FrameId;
		std::string FrameName = Name + FrameId;
		
		//Frame F = Frame()
	}
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
	if (mCurrentClip.Frames.size() == 0) return;

	// Update the current clip
	mCurrentClip.CurrFrameTime += DeltaTime;
	if (mCurrentClip.CurrFrameTime > mCurrentClip.FrameTime)
	{
		mCurrentClip.CurrFrameTime = 0.f;
		mCurrentClip.CurrFrameIndex++;
		if (mCurrentClip.CurrFrameIndex == mCurrentClip.Frames.size())
		{
			mCurrentClip.CurrFrameIndex = 0;
		}
	}

	SetFrame(mCurrentClip.Frames[mCurrentClip.CurrFrameIndex]);
}

void Core::AnimationComponent::Destroy()
{
	AtlasComponent::Destroy();
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
		
		if (mFrames.count(InName) == 0)
		{
			for (Frame F : InClip.Frames)
			{
				mFrames[F.Name] = F;
			}
		}
	}
}

void Core::AnimationComponent::SetClip(const std::string& InClipName)
{
	if (mClips.count(InClipName) == 0) return;
	mCurrentClip.Reset();
	mCurrentClip = mClips[InClipName];
}