#include "Components/PlayerComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IInput.h"
#include "Object.h"
#include "Components/PhysicComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"

Core::PlayerComponent::PlayerComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PlayerComponent::Start()
{
	if (!mOwner) return;
	AdjustBoxSize();

	mAnimationComponent = mOwner->GetComponent<AnimationComponent>();
	mPxComponent = mOwner->GetComponent<PhysicComponent>();

	if (AttributeComponent* AttributeComp = mOwner->GetComponent<AttributeComponent>())
	{
		AttributeComp->mOnHealthChangedSubject.AddListener(this);
		float PlayerHealth = World().GetPersistentValueF("PlayerHealth");
		if (PlayerHealth)
		{
			AttributeComp->SetCurrentHealth(PlayerHealth);
		}
		else
		{
			AttributeComp->SetCurrentHealth(AttributeComp->GetMaxHealth());
		}
	}
}

void Core::PlayerComponent::Update(float DeltaTime)
{
	if (!CheckReferences()) return;
	if (!bAlive) return;

	Vector<float> MovementInput = Vector<float>::ZeroVector();
	GetMovementInput(MovementInput);
	mWantToRoll = Input().IsKeyDown(EKey::Space);

	mCurrentVelocity = mPxComponent->GetVelocity();
	UpdateFlip();
	CheckRoll();

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

void Core::PlayerComponent::DeathEndNotify()
{
	World().SetPersistentValueF("PlayerHealth", 100.f);
	World().Load("MenuScene");
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

void Core::PlayerComponent::UpdateFlip()
{
	Flip NewFlip = mCurrentVelocity.X < 0.f ? Flip(true, false) : Flip(false, false);
	mAnimationComponent->SetFlip(NewFlip);
}

void Core::PlayerComponent::CheckRoll()
{
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
		if (mCurrentVelocity.Length() > 0.f)
		{
			mAnimationComponent->SetClip("Walk", true);
		}
		else
		{
			mAnimationComponent->SetClip("Idle", true);
		}
	}
}

void Core::PlayerComponent::AdjustBoxSize()
{
	BoxComponent* Box = mOwner->GetComponent<BoxComponent>();
	if (Box)
	{
		Box->SetBoxSize(mOwner->GetSize().X * 0.3f, mOwner->GetSize().Y * 0.4f);
		Box->SetOffset(mOwner->GetSize().X * 0.34f, mOwner->GetSize().Y * 0.48f);

		// adjust position
		mOwner->SetLocation(mOwner->GetLocation() + Box->GetCollisionLocation());
	}
}

void Core::PlayerComponent::Die()
{
	if (!bAlive) return;
	bAlive = false;
	Audio().PlaySFX(mDeathSoundId);
	if (mAnimationComponent)
	{
		mAnimationComponent->SetClip("Death", false, std::bind(&PlayerComponent::DeathEndNotify, this));
	}
}

void Core::PlayerComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	void* HealthVoidPtr = nullptr;

	if (Value.count("Health") > 0)
	{
		HealthVoidPtr = Value.at("Health");
	}

	if (HealthVoidPtr)
	{
		float* Health = static_cast<float*>(HealthVoidPtr);
		if (Health)
		{
			World().SetPersistentValueF("PlayerHealth", *Health);

			if (*Health == 0)
			{
				// die
				Die();
			}
		}
	}
}

void Core::PlayerComponent::AddHitSoundId(const std::vector<size_t>& InId)
{
	mHitSoundId = InId;
}

void Core::PlayerComponent::SetDeathSoundId(size_t InId)
{
	mDeathSoundId = InId;
}

void Core::PlayerComponent::Destroy()
{
}

Core::Component* Core::PlayerComponent::Clone(Object* Owner)
{
	PlayerComponent* Clone = new PlayerComponent(Owner);
	__super::SetupClone(Clone);
	Clone->mHitSoundId = mHitSoundId;
	Clone->mDeathSoundId = mDeathSoundId;
	return Clone;
}

void Core::PlayerComponent::SetupClone(Component* Child)
{
	PlayerComponent* Clone = dynamic_cast<PlayerComponent*>(Child);
	__super::SetupClone(Clone);
	if (!Clone) return;
	Clone->mHitSoundId = mHitSoundId;
	Clone->mDeathSoundId = mDeathSoundId;
}
