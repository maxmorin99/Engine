#include "Components/EnemyComponent.h"
#include "Components/PathFindingComponent.h"
#include "Object.h"
#include "Components/AnimationComponent.h"

EnemyComponent::EnemyComponent(Object* Owner):
	Component(Owner)
{
}

void EnemyComponent::Start()
{
	Component::Start();

	if (mOwner)
	{
		mPathFindingComp = mOwner->GetComponent<PathFindingComponent>();
	}

	Target = World().GetObjectsWithTag("Player")[0];
}

void EnemyComponent::Update(float DeltaTime)
{
	// Update Animation based on enemy's velocity
	UpdateAnimation();

	// Update Flip based on movement dir
	UpdateFlip();

	if (Target)
	{
		mPath = mPathFindingComp->GetPath(Target->GetCenterLocation());
		bHasPath = true;
	}
	mPathFindingComp->Move();
}

void EnemyComponent::UpdateAnimation()
{
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
