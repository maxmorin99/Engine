#include "Components/SpriteComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IGraphic.h"
#include "Object.h"

Core::SpriteComponent::SpriteComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::SpriteComponent::Start()
{
	mTextureId = Graphic().LoadTexture(mFile.c_str());
	Graphic().GetTextureSize(mTextureId, &mSrc.W, &mSrc.H);
}

void Core::SpriteComponent::Destroy()
{
}

void Core::SpriteComponent::Draw()
{
	if (!mOwner) return;

	Vector<float> OwnerSize = mOwner->GetSize();
	Vector<float> OwnerLoc = mOwner->GetLocation();
	mDst = Rect<int>(static_cast<int>(OwnerLoc.X), static_cast<int>(OwnerLoc.Y), static_cast<int>(OwnerSize.X), static_cast<int>(OwnerSize.Y));
	Graphic().SetDrawColor(mColor);
	Graphic().DrawTexture(mTextureId, mSrc, mDst, 0, Flip::None, Color::White);
}