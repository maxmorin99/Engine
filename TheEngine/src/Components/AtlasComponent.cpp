#include "Components/AtlasComponent.h"
#include "Object.h"

/* struct Frame ------------------------------------------------------ */

Core::Frame::Frame() :
	Rect(0, 0, 0, 0), Name("") {}

Core::Frame::Frame(int X, int Y, int W, int H, const std::string& InName) :
	Rect(X, Y, W, H), Name(InName) {}

Core::Frame& Core::Frame::Empty()
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

void Core::AtlasComponent::AddFrame(const std::string& Name, int X, int Y, int W, int H)
{
	if (mFrames.count(Name) > 0) return;
	int FrameX = X * W;
	int FrameY = Y * H;
	mFrames[Name] = Frame(FrameX, FrameY, W, H, Name);
}

void Core::AtlasComponent::SetFrame(const std::string& Name)
{
	if (mFrames.count(Name) == 0) return;
	mCurrentFrame = mFrames[Name];
	mSrc = mCurrentFrame.Rect;
}

void Core::AtlasComponent::Destroy()
{
	SpriteComponent::Destroy();
}
