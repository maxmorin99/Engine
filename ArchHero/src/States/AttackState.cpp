#include "States/AttackState.h"
#include "Components/EnemyComponent.h"
#include "Engine/Engine.h"
#include "Object.h"

AttackState::AttackState()
{
}

void AttackState::OnEnter(EnemyComponent* Enemy)
{
}

void AttackState::Execute(EnemyComponent* Enemy)
{
	if (!Enemy) return;
	Object* Owner = Enemy->GetOwner();
	Object* Target = Enemy->GetTarget();

	if (Owner && Target)
	{
		float Dist = Vector<float>::Dist(Owner->GetCenterLocation(), Target->GetCenterLocation());
		if (Dist > Enemy->GetToleranceDistance())
		{
			Enemy->ChangeState("Chase");
		}
	}
}

void AttackState::OnExit(EnemyComponent* Enemy)
{
}
