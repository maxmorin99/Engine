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
	if (!Owner || !Target) return;

	float EnemyAttackDelay = Enemy->GetAttackDelay();
	float EnemyCurrAttackDelay = Enemy->GetCurrAttackDelay();

	if (EnemyCurrAttackDelay > EnemyAttackDelay)
	{
		SpawnBullet(Enemy, Target, Owner);
	}
	
	if (Owner && Target)
	{
		float Dist = Vector<float>::Dist(Owner->GetCenterLocation(), Target->GetCenterLocation());
		if (Dist > Enemy->GetToleranceDistance())
		{
			Enemy->SetCurrAttackDelay(Enemy->GetAttackDelay());
			Enemy->ChangeState("Chase");
		}
	}
}

void AttackState::OnExit(EnemyComponent* Enemy)
{
}

void AttackState::SpawnBullet(EnemyComponent* Enemy, Object* Target, Object* Owner)
{
	Enemy->SetCurrAttackDelay(0.f);
	Object* Bullet = Engine::GetSpawner().Spawn("EnemyBullet");
	Vector<float> ToTargetDir = Vector<float>(Target->GetCenterLocation() - Owner->GetCenterLocation()).GetNormalized();
	float RotationRad = ToTargetDir.GetRotationFromX();
	float RotationDeg = RotationRad * 180 / PI;

	if (Target->GetCenterLocation().Y > Owner->GetCenterLocation().Y)
	{
		RotationDeg *= -1;
	}

	Bullet->SetRotation(RotationDeg);
	Bullet->SetCenterLocation(Owner->GetCenterLocation());

	Engine::GetWorld().AddObject(Bullet);
}
