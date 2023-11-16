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
	if (mPathFindingComp)
	{
		Object* Player = World().GetObjectsWithTag("Player")[0];
		Vector<float> PlayerLoc = Player->GetCenterLocation();
		std::vector<Vector<float>> Path = mPathFindingComp->GetPath(PlayerLoc);
		int bob;
	}
}

void EnemyComponent::Destroy()
{
	Component::Destroy();
}

void EnemyComponent::Draw()
{
	Graphic().DrawLineF(Vector<float>::ZeroVector(), mOwner->GetCenterLocation(), Color::Red);
}
