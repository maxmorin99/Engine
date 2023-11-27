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
	Component::Start();
}

void Core::SpriteComponent::Draw()
{
	if (!mOwner) return;

	Vector<float> OwnerSize = mOwner->GetSize();
	Vector<float> OwnerLoc = mOwner->GetLocation();
	float OwnerRot = mOwner->GetRotation();
	mDst = Rect<int>(static_cast<int>(OwnerLoc.X), static_cast<int>(OwnerLoc.Y), static_cast<int>(OwnerSize.X), static_cast<int>(OwnerSize.Y));
	Graphic().DrawTexture(mTextureId, mSrc, mDst, OwnerRot, mFlip, mColor);
}

Core::Component* Core::SpriteComponent::Clone(Object* Owner)
{
	SpriteComponent* Clone = new SpriteComponent(Owner);
	__super::SetupClone(Clone);

	Clone->mFile = mFile;
	Clone->mTextureId = mTextureId;
	Clone->mSrc = mSrc;
	Clone->mDst = mDst;
	Clone->mColor = mColor;
	Clone->mFlip = mFlip;
	Clone->mCenterOffset = mCenterOffset;
	return Clone;
}

void Core::SpriteComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);

	SpriteComponent* Clone = dynamic_cast<SpriteComponent*>(Child);
	if (!Clone) return;

	Clone->mFile = mFile;
	Clone->mTextureId = mTextureId;
	Clone->mSrc = mSrc;
	Clone->mDst = mDst;
	Clone->mColor = mColor;
	Clone->mFlip = mFlip;
	Clone->mCenterOffset = mCenterOffset;
}

void Core::SpriteComponent::SetFile(const std::string& File)
{
	mFile = File;
	mTextureId = Graphic().LoadTexture(mFile.c_str());
	Graphic().GetTextureSize(mTextureId, &mSrc.W, &mSrc.H);
}

void Core::SpriteComponent::SetFile(const std::string& File, int IdxX, int IdxY, int ImgCountW, int ImgCountH)
{
	mFile = File;
	mTextureId = Graphic().LoadTexture(mFile.c_str());
	int SizeX = 0;
	int SizeY = 0;
	Graphic().GetTextureSize(mTextureId, &SizeX, &SizeY);
	Logger().DebugLog(ConsoleColor::Yellow, "SizeW: %d, SizeH: %d\n", SizeX / ImgCountW, SizeY / ImgCountH);
	mSrc = Rect<int>(IdxX * SizeX / ImgCountW, IdxY * SizeY / ImgCountH, SizeX / ImgCountW, SizeY / ImgCountH);
}

void Core::SpriteComponent::SetFlip(const Flip& InFlip)
{
	mFlip = InFlip;
}

void Core::SpriteComponent::Destroy()
{
	Component::Destroy();
}
