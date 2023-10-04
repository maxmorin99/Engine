#include "Components/AnimationComponent.h"
#include "Object.h"

Core::AnimationComponent::AnimationComponent(Object* Owner):
	Component(Owner),
	mTextureData(TextureData::Empty())
{
}

void Core::AnimationComponent::Start()
{
	mTextureId = Graphic().LoadTexture(mTextureData.File.c_str());
	Graphic().GetTextureSize(mTextureId, &mTextureSize.X, &mTextureSize.Y);

	mSrc.W = mTextureSize.X / mTextureData.Col;
	mSrc.H = mTextureSize.Y / mTextureData.Rows;
	mSrc.Y = mTextureData.Index / mTextureData.Col;
	mSrc.X = mTextureData.Index - mSrc.Y * mTextureData.Col;
	mSrc.Y *= (mSrc.H - mTextureData.Padding);
	mSrc.X *= (mSrc.W - mTextureData.Padding);
}

void Core::AnimationComponent::Destroy()
{
}

void Core::AnimationComponent::Draw()
{
	if (!mOwner) return;

	Vector<float> OwnerSize = mOwner->GetSize();
	Vector<float> OwnerLoc = mOwner->GetLocation();
	Rect<int> Dst(static_cast<int>(OwnerLoc.X), static_cast<int>(OwnerLoc.Y), static_cast<int>(OwnerSize.X), static_cast<int>(OwnerSize.Y));
	mDst = Dst;
	Graphic().SetDrawColor(mColor);
	//Graphic().DrawTexture(mTextureId, Dst, mColor);
	Graphic().DrawTexture(mTextureId, mSrc, mDst, 0, Flip::None, Color::White);
}
