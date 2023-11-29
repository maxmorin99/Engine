#include "States/ChaseState.h"
#include "Components/EnemyComponent.h"
#include "Components/PathFindingComponent.h"
#include "Object.h"
#include <vector>

ChaseState::ChaseState()
{
}

void ChaseState::OnEnter(EnemyComponent* Enemy)
{
}

void ChaseState::Execute(EnemyComponent* Enemy)
{
	if (!Enemy) return;
	Object* Owner = Enemy->GetOwner();
	if (!Owner) return;
	Object* Target = Enemy->GetTarget();
	if (!Target) return;
	PathFindingComponent* PathFindingComp = Owner->GetComponent<PathFindingComponent>();
	if (!PathFindingComp) return;

	std::vector<Vector<float>> Path;

	if (Enemy->GetTarget())
	{
		Path = PathFindingComp->GetPath(Enemy->GetTarget()->GetCenterLocation());
	}
	PathFindingComp->Move();

	float Dist = Vector<float>::Dist(Owner->GetCenterLocation(), Target->GetCenterLocation());
	if (Dist < Enemy->GetToleranceDistance())
	{
		Enemy->ChangeState("Attack");
	}
}

void ChaseState::OnExit(EnemyComponent* Enemy)
{
}


