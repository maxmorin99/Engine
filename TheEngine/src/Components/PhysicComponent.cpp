#include "Components/PhysicComponent.h"
#include "Object.h"
#include <algorithm>


Core::PhysicComponent::PhysicComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PhysicComponent::Start()
{
}

void Core::PhysicComponent::Update(float DeltaTime)
{
	if (!mOwner) return;
	mVelocity.X += mPendingMove.X * DeltaTime;
	mVelocity.Y -= mPendingMove.Y * DeltaTime;

	// keep the velocity under the max movement speed
	if (mVelocity.Length() > mMaxMovementSpeed)
	{
		mVelocity = mVelocity.GetNormalized() * mMaxMovementSpeed;
	}

	Vector<float> OwnerPos = mOwner->GetLocation();
	Vector<float> NewPos = OwnerPos + mVelocity * DeltaTime;
	mOwner->SetLocation(NewPos);

	if (mPendingMove.X == 0.f)
	{
		if (mVelocity.X < 0.f)
		{
			mVelocity.X += mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.X = std::clamp(mVelocity.X, -10000.f, 0.f);
		}
		else if (mVelocity.X > 0.f)
		{
			mVelocity.X -= mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.X = std::clamp(mVelocity.X, 0.f, 100000.f);
		}
	}
	if (mPendingMove.Y == 0.f)
	{
		if (mVelocity.Y < 0.f)
		{
			mVelocity.Y += mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.Y = std::clamp(mVelocity.Y, -10000.f, 0.f);
		}
		else if (mVelocity.Y > 0.f)
		{
			mVelocity.Y -= mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.Y = std::clamp(mVelocity.Y, 0.f, 10000.f);
		}
	}

	mPendingMove = Vector<float>::ZeroVector();
}

void Core::PhysicComponent::AddMovement(const Vector<float>& MovementDirection)
{
	if (MovementDirection.Length() == 0.f) return;
	const Vector<float> NormalizedMovementDirection = MovementDirection.GetNormalized();
	mPendingMove = NormalizedMovementDirection * mAccelerationSpeed / (mMass * mSlideFactor);
}

void Core::PhysicComponent::SetSlideFactor(float Factor)
{
	mSlideFactor = Factor == 0.f ? 1.f : Factor;
}

void Core::PhysicComponent::SetMass(float Mass)
{
	mMass = Mass == 0.f ? 1.f : Mass;
}

void Core::PhysicComponent::Destroy()
{
}
