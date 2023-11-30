#include "Components/EnemyComponent.h"
#include "Components/PathFindingComponent.h"
#include "Object.h"
#include "Components/AnimationComponent.h"
#include "States/AttackState.h"
#include "States/ChaseState.h"
#include "States/DeathState.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"

EnemyComponent::EnemyComponent(Object* Owner):
	Component(Owner)
{
	mStates["Attack"] = new AttackState();
	mStates["Chase"] = new ChaseState();
	mStates["Death"] = new DeathState();
	ChangeState("Chase");
}

void EnemyComponent::Start()
{
	Component::Start();

	AdjustBoxSize();

	BindAttributeSubject();

	mCurrAttackDelay = mAttackDelay;

	if (mOwner)
	{
		mPathFindingComp = mOwner->GetComponent<PathFindingComponent>();
	}

	mTarget = World().GetObjectsWithTag("Player")[0];
}

void EnemyComponent::Update(float DeltaTime)
{
	// Update Animation based on enemy's velocity
	UpdateAnimation();

	// Update Flip based on movement dir
	UpdateFlip();

	mCurrAttackDelay += DeltaTime;

	if (mCurrentState)
	{
		mCurrentState->Execute(this);
	}
}

void EnemyComponent::UpdateAnimation()
{
	if (mCurrentState && mCurrentState == mStates["Death"]) return;
	if (mOwner)
	{
		AnimationComponent* AnimComp = mOwner->GetComponent<AnimationComponent>();
		if (AnimComp)
		{
			if (mOwner->GetVelocity().Length() > 0.f)
			{
				AnimComp->SetClip("Walk", true);
			}
			else if (AnimComp->GetCurrentClipName() != "Idle")
			{
				AnimComp->SetClip("Idle", true);
			}
		}
	}
}

void EnemyComponent::UpdateFlip()
{
	if (mOwner)
	{
		if (mOwner->GetVelocity().X < 0)
		{
			mOwner->SetFlip(Flip(true, false));
		}
		else if (mOwner->GetVelocity().X > 0)
		{
			mOwner->SetFlip(Flip(false, false));
		}
		else
		{
			if (GetTarget())
			{
				if (GetTarget()->GetLocation().X < mOwner->GetLocation().X)
				{
					mOwner->SetFlip(Flip(true, false));
				}
				else
				{
					mOwner->SetFlip(Flip(false, false));
				}
			}
		}
	}
}

void EnemyComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	void* HealthVoidPtr = nullptr;
	if (Value.count("Health") > 0)
	{
		HealthVoidPtr = Value.at("Health");
	}
	if (!HealthVoidPtr) return;
	
	if (float* Health = static_cast<float*>(HealthVoidPtr))
	{
		if (*Health == 0)
		{
			ChangeState("Death");
		}
	}

}

void EnemyComponent::ChangeState(const std::string& StateName)
{
	if (mStates.count(StateName) == 0) return;

	if (mCurrentState)
	{
		mCurrentState->OnExit(this);
	}
	mCurrentState = mStates.at(StateName);
	if (mCurrentState)
	{
		mCurrentState->OnEnter(this);
	}
}

void EnemyComponent::Draw()
{
	for (int i = 0; i < mPath.size(); i++)
	{
		Vector<float> prev = Vector<float>::ZeroVector();
		Vector<float> curr = mPath[i];
		Color c = Color::Green;
		if (i == 0)
		{
			prev = mOwner->GetSpriteCenterLocation();
			c = Color::Yellow;
		}
		else
		{
			prev = mPath[i - 1];
		}
		Graphic().DrawLineF(prev, curr, c);
	}
}

void EnemyComponent::Destroy()
{
	Component::Destroy();

	for (auto& pair : mStates)
	{
		delete pair.second;
	}
}

Component* EnemyComponent::Clone(Object* Owner)
{
	EnemyComponent* Clone = new EnemyComponent(Owner);
	__super::SetupClone(Clone);

	Clone->mToleranceDistance = mToleranceDistance;
	Clone->mAttackDelay = mAttackDelay;
	Clone->mCurrAttackDelay = mCurrAttackDelay;
	return Clone;
}

void EnemyComponent::SetupClone(Component* Child)
{
	EnemyComponent* Clone = dynamic_cast<EnemyComponent*>(Child);
	__super::SetupClone(Child);
	if (!Clone) return;

	Clone->mToleranceDistance = mToleranceDistance;
	Clone->mAttackDelay = mAttackDelay;
	Clone->mCurrAttackDelay = mCurrAttackDelay;
}

void EnemyComponent::BindAttributeSubject()
{
	if (!mOwner) return;
	AttributeComponent* AttributeComp = mOwner->GetComponent<AttributeComponent>();
	if (!AttributeComp) return;
	AttributeComp->mOnHealthChangedSubject.AddListener(this);
}

void EnemyComponent::AdjustBoxSize()
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
