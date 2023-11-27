#include "Components/AtlasComponent.h"
#include "Object.h"

/* struct Frame ------------------------------------------------------ */

Core::Frame::Frame() :
	Rect(0, 0, 0, 0), Name("") {}

Core::Frame::Frame(int X, int Y, int W, int H, const std::string& InName) :
	Rect(X, Y, W, H), Name(InName) {}

Core::Frame Core::Frame::Empty()
{
	return Frame();
}


/* Atlas Component --------------------------------------------------- */

Core::AtlasComponent::AtlasComponent(Object* Owner) :
	SpriteComponent(Owner)
{
}

void Core::AtlasComponent::Start()
{
	SpriteComponent::Start();
}

void Core::AtlasComponent::Draw()
{
	SpriteComponent::Draw();
}

Core::Component* Core::AtlasComponent::Clone(Object* Owner)
{
	AtlasComponent* Clone = new AtlasComponent(Owner);
	__super::SetupClone(Clone);

	Clone->mTextureSize = mTextureSize;
	Clone->mFrames = mFrames;
	Clone->mCurrentFrame = mCurrentFrame;

	return Clone;
}

void Core::AtlasComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);

	AtlasComponent* Clone = dynamic_cast<AtlasComponent*>(Child);
	if (!Clone) return;

	Clone->mTextureSize = mTextureSize;
	Clone->mFrames = mFrames;
	Clone->mCurrentFrame = mCurrentFrame;
}

void Core::AtlasComponent::AddFrame(const std::string& Name, int X, int Y, int W, int H)
{
	if (mFrames.count(Name) > 0) return;
	// scale X and Y by texture W and H
	int FrameX = X * W;
	int FrameY = Y * H;
	mFrames[Name] = Frame(FrameX, FrameY, W, H, Name);
}

void Core::AtlasComponent::AddFrame(Frame& InFrame)
{
	if (mFrames.count(InFrame.Name) > 0) return;
	// scale X and Y by texture W and H
	InFrame.Rect.X *= InFrame.Rect.W;
	InFrame.Rect.Y *= InFrame.Rect.H;
	mFrames[InFrame.Name] = InFrame;
}

void Core::AtlasComponent::SetFrame(const std::string& Name)
{
	if (mFrames.count(Name) == 0) return;
	mCurrentFrame = mFrames[Name];
	mSrc = mCurrentFrame.Rect;
}

void Core::AtlasComponent::SetFrame(Frame& InFrame)
{
	bool bCountains = false;
	for (auto& Pair : mFrames)
	{
		Frame F = Pair.second;
		if (F.Name == InFrame.Name)
		{
			bCountains = true;
			break;
		}
	}
	if (!bCountains) return;

	mCurrentFrame = InFrame;
	mSrc = mCurrentFrame.Rect;
}

void Core::AtlasComponent::Destroy()
{
	SpriteComponent::Destroy();
}
