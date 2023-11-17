#include "Components/EnemyComponent.h"
#include "Components/PathFindingComponent.h"
#include "Object.h"

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
}

void EnemyComponent::Update(float DeltaTime)
{
	Object* Player = World().GetObjectsWithTag("Player")[0];
	if (Player)
	{
		mPath = mPathFindingComp->GetPath(Player->GetCenterLocation());
		bHasPath = true;
	}
}

void EnemyComponent::Draw()
{
	Graphic().DrawLineF(Vector<float>::ZeroVector(), mOwner->GetCenterLocation(), Color::Red);
	for (int i = 0; i < mPath.size(); i++)
	{
		Vector<float> prev = Vector<float>::ZeroVector();
		Vector<float> curr = mPath[i];
		Color c = Color::Green;
		if (i == 0)
		{
			prev = mOwner->GetCenterLocation();
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
