#include "Components/PlayerComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IInput.h"
#include "Object.h"
#include "Components/PhysicComponent.h"
#include "Components/AnimationComponent.h"

Core::PlayerComponent::PlayerComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PlayerComponent::Start()
{
	if (!mOwner) return;
	mAnimationComponent = mOwner->GetComponent<AnimationComponent>();
	mPxComponent = mOwner->GetComponent<PhysicComponent>();
}

void Core::PlayerComponent::Update(float DeltaTime)
{
	if (!CheckReferences()) return;

	Vector<float> MovementInput = Vector<float>::ZeroVector();
	GetMovementInput(MovementInput);
	mWantToRoll = Input().IsKeyDown(EKey::Space);

	Vector<float> CurrentVelocity = Vector<float>::ZeroVector();
	CurrentVelocity = mPxComponent->GetVelocity();

	Flip NewFlip = CurrentVelocity.X < 0.f ? Flip(true, false) : Flip(false, false);
	mAnimationComponent->SetFlip(NewFlip);

	if (mWantToRoll)
	{
		if (!mRolling)
		{
			mRolling = true;
			mAnimationComponent->SetClip("Roll", false, std::bind(&Core::PlayerComponent::RollEndNotify, this));
		}
	}
	else if (!mRolling)
	{
		if (CurrentVelocity.Length() > 0.f)
		{
			mAnimationComponent->SetClip("Walk", true);
		}
		else
		{
			mAnimationComponent->SetClip("Idle", true);
		}
	}

	mPxComponent->AddMovement(MovementInput);
}

void Core::PlayerComponent::GetMovementInput(Vector<float>& OutMovementInput)
{
	if (Input().IsKeyDown(EKey::D))
	{
		OutMovementInput.X += 1.f;
	}
	if (Input().IsKeyDown(EKey::A))
	{
		OutMovementInput.X -= 1.f;
	}
	if (Input().IsKeyDown(EKey::W))
	{
		OutMovementInput.Y += 1.f;
	}
	if (Input().IsKeyDown(EKey::S))
	{
		OutMovementInput.Y -= 1.f;
	}
}

void Core::PlayerComponent::RollEndNotify()
{
	mRolling = false;
}

bool Core::PlayerComponent::CheckReferences() const
{
	if (!mAnimationComponent)
	{
		//Logger().DebugLog(ConsoleColor::Red, "Player's AnimationComponent is not valid");
		return false;
	}
	if (!mPxComponent)
	{
		//Logger().DebugLog(ConsoleColor::Red, "Player's PxComponent is not valid");
		return false;
	}
	if (!mOwner)
	{
		//Logger().DebugLog(ConsoleColor::Red, "Player's Owner is not valid");
		return false;
	}
	return true;
}

void Core::PlayerComponent::Destroy()
{
}
