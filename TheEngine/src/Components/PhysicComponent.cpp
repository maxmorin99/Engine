#include "Components/PhysicComponent.h"
#include "Object.h"

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
	Vector<float> OwnerPos = mOwner->GetLocation();
	Vector<float> NewPos = OwnerPos + mVelocity * DeltaTime;
	mOwner->SetLocation(NewPos);
}

void Core::PhysicComponent::AddMovement(const Vector<float>& MovementDirection)
{
	const Vector<float> NormalizedMovementDirection = MovementDirection.GetNormalized();
	mVelocity = mVelocity + NormalizedMovementDirection * mMovementSpeed / mMass;
	if (mVelocity.Length() > mMaxMovementSpeed)
	{
		mVelocity = mVelocity.GetNormalized() * mMaxMovementSpeed;
	}
}

void Core::PhysicComponent::SetMass(float Mass)
{
	mMass = Mass == 0.f ? 1.f : Mass;
}

void Core::PhysicComponent::Destroy()
{
}
