#include "Components/WeaponComponent.h"
#include "Object.h"
#include "Components/AnimationComponent.h"
#include "Math/RotationMatrix.h"

WeaponComponent::WeaponComponent(Object* InOwner) :
	SpriteComponent(InOwner)
{
}

void WeaponComponent::Update(float DeltaTime)
{
	if (!mInstigator || !mOwner) return;
	
	UpdateFlip();
	UpdateWeaponLocation();
	UpdateRotation();
}

void WeaponComponent::Draw()
{
	SpriteComponent::Draw();

}

void WeaponComponent::UpdateFlip()
{
	// calculate angle between target and instigator (player)
	float AngleBetweentargetAndInstigator = mDirFromInstigator.GetNormalized().GetRotationFromX();
	float AngleDeg = AngleBetweentargetAndInstigator * 180 / PI;
	if (AngleDeg < 90)
	{
		mFlip.V = false;
		mInstigator->SetFlip(Flip(false, false));
	}
	else
	{
		mFlip.V = true;
		mInstigator->SetFlip(Flip(true, false));
	}
}

void WeaponComponent::UpdateWeaponLocation()
{
	// Put the weapon to the left or right of its instigator dependinf on its Flip

	Vector<float> InstigatorLoc = mInstigator->GetLocation();
	Vector<float> InstigatorSize = mInstigator->GetSize();
	Vector<float> OwnerSize = mOwner->GetSize();

	float OffsetX = mFlip.V || mFlip.H ? -mOffset.X : mOffset.X;
	Vector<float> InstigatorCenter = Vector<float>(InstigatorLoc.X + InstigatorSize.X / 2, InstigatorLoc.Y + InstigatorSize.Y / 2);
	Vector<float> AdjustedPos = Vector<float>(InstigatorCenter.X - OwnerSize.X / 2 + OffsetX, InstigatorCenter.Y + mOffset.Y);

	mOwner->SetLocation(AdjustedPos);
}

void WeaponComponent::UpdateRotation()
{
	if (!mTargetCursor) return;

	Vector<float> TargetCenter = Vector<float>(
		mTargetCursor->GetLocation().X + mTargetCursor->GetSize().X / 2,
		mTargetCursor->GetLocation().Y + mTargetCursor->GetSize().Y / 2
	);

	Vector<float> OwnerCenter = Vector<float>(
		mOwner->GetLocation().X + mOwner->GetSize().X / 2,
		mOwner->GetLocation().Y + mOwner->GetSize().Y / 2
	);

	Vector<float> InstigatorCenter = Vector<float>(
		mInstigator->GetLocation().X + mInstigator->GetSize().X / 2,
		mInstigator->GetLocation().Y + mInstigator->GetSize().Y / 2
	);

	mDirFromOwner = TargetCenter - OwnerCenter;
	mDirFromInstigator = TargetCenter - InstigatorCenter;

	float AngleRad = mDirFromOwner.GetNormalized().GetRotationFromX();
	float AngleDeg = AngleRad * 180 / PI;

	if (TargetCenter.Y < OwnerCenter.Y)
	{
		AngleDeg = 360 - AngleDeg;
	}

	mOwner->SetRotation(AngleDeg);
}
