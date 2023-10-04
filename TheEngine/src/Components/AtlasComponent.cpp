#include "Components/AtlasComponent.h"
#include "Object.h"

Core::AtlasComponent::AtlasComponent(Object* Owner) :
	SpriteComponent(Owner)
{
}

void Core::AtlasComponent::Start()
{
	SpriteComponent::Start();

	Graphic().GetTextureSize(mTextureId, &mTextureSize.X, &mTextureSize.Y);
}

void Core::AtlasComponent::Draw()
{
	SpriteComponent::Draw();
}

void Core::AtlasComponent::AddFrame(const std::string& Name, int X, int Y, int W, int H)
{
	if (mFrames.count(Name) > 0) return;
	mFrames[Name] = Rect<int>(X, Y, W, H);
}

void Core::AtlasComponent::SetFrame(const std::string& Name)
{
	mSrc = mFrames[Name];
}

void Core::AtlasComponent::Destroy()
{
}
