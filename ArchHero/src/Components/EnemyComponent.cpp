#include "Components/EnemyComponent.h"
#include "Components/PathFindingComponent.h"
#include "Object.h"
#include "Components/AnimationComponent.h"
#include "States/AttackState.h"
#include "States/ChaseState.h"
#include "States/DeathState.h"

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

	if (mCurrentState)
	{
		mCurrentState->Execute(this);
	}

	//ChaseTarget();
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

void EnemyComponent::Attack()
{
}

void EnemyComponent::Draw()
{
	Graphic().DrawLineF(Vector<float>::ZeroVector(), mOwner->GetSpriteCenterLocation(), Color::Red);
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

	return Clone;
}

void EnemyComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);
}

void EnemyComponent::ChaseTarget()
{
	if (mTarget)
	{
		mPath = mPathFindingComp->GetPath(mTarget->GetCenterLocation());
		bHasPath = true;
	}
	mPathFindingComp->Move();

	if (mTarget)
	{
		float Dist = Vector<float>::Dist(mOwner->GetCenterLocation(), mTarget->GetCenterLocation());
		if (Dist < mToleranceDistance)
		{
			// change to attack state
			ChangeState("Attack");
		}
	}
}
