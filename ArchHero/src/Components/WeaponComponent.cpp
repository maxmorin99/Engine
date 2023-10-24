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

	Graphic().DrawLineF(TestDir[0], TestDir[1], Color::Green);
}

void WeaponComponent::UpdateFlip()
{
	// Flip weapon sprite based on instigator vel and weapon rotation
	Vector<float> InstigatorVelocity = mInstigator->GetVelocity();
	if (InstigatorVelocity.X < 0.f)
	{
		mFlip.V = mOwner->GetRotation() <= 90.f && mOwner->GetRotation() > 270.f ? false : true;
		mFlip.H = !mFlip.V;
	}
	else
	{
		mFlip.H = false;
		mFlip.V = false;
	}
}

void WeaponComponent::UpdateWeaponLocation()
{
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

	Vector<float> Dir = TargetCenter - OwnerCenter;
	float Distance = Dir.Length();
	
	TestDir[0] = OwnerCenter;
	TestDir[1] = OwnerCenter + Dir.GetNormalized() * Distance;

	float AngleRad = Dir.GetNormalized().GetRotationFromX();
	float AngleDeg = AngleRad * 180 / PI;

	Logger().DebugLog(ConsoleColor::Yellow, "AngleDeg: %f\n", AngleDeg);
	mOwner->SetRotation(AngleDeg);
}

float WeaponComponent::Dot(const Vector<float>& Vec1, const Vector<float>& Vec2)
{
	return (Vec1.X * Vec2.X) + (Vec1.Y * Vec2.Y);
}
