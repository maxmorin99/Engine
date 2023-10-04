#include "Components/SpriteComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IGraphic.h"
#include "Object.h"

Core::SpriteComponent::SpriteComponent(Object* Owner) :
	Component(Owner),
	mTextureData(TextureData::Empty())
{
}

Core::SpriteComponent::SpriteComponent(Object* Owner, const TextureData& Data) :
	Component(Owner),
	mTextureData(Data)
{
}

void Core::SpriteComponent::Start()
{
	mTextureId = Graphic().LoadTexture(mTextureData.File.c_str());
	Graphic().GetTextureSize(mTextureId, &mTextureSize.X, &mTextureSize.Y);
	
	mSrc.W = mTextureSize.X / mTextureData.Col;
	mSrc.H = mTextureSize.Y / mTextureData.Rows;
	mSrc.Y = mTextureData.Index / mTextureData.Col;
	mSrc.X = mTextureData.Index - mSrc.Y * mTextureData.Col;
	mSrc.Y *= mSrc.H;
	mSrc.X *= mSrc.W;
}

void Core::SpriteComponent::Destroy()
{
}

void Core::SpriteComponent::Draw()
{
	if (!mOwner) return;

	Vector<float> OwnerSize = mOwner->GetSize();
	Vector<float> OwnerLoc = mOwner->GetLocation();
	Rect<int> Dst(static_cast<int>(OwnerLoc.X), static_cast<int>(OwnerLoc.Y), static_cast<int>(OwnerSize.X), static_cast<int>(OwnerSize.Y));
	
	Graphic().SetDrawColor(mColor);
	//Graphic().DrawTexture(mTextureId, Dst, mColor);
	Graphic().DrawTexture(mTextureId, mSrc, Dst, 0, Flip::None, Color::White);
}