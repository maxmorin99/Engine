#include "States/DeathState.h"
#include "Object.h"
#include "Components/AnimationComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/CollisionComponent.h"
#include "Engine/Engine.h"

DeathState::DeathState()
{
}

void DeathState::OnEnter(EnemyComponent* Enemy)
{
	if (!Enemy) return;
	Owner = Enemy->GetOwner();
	if (!Owner) return;
	AnimationComponent* AnimComp = Owner->GetComponent<AnimationComponent>();
	if (!AnimComp) return;
	AnimComp->SetColor(AnimComp->GetDeathAnimationColor());
	AnimComp->SetClip("Death", false, std::bind(&DeathState::OnDeathAnimationEnd, this));

	// disbale collision
	CollisionComponent* CollisionComp = Owner->GetCollisionComponent();
	if (CollisionComp)
	{
		CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);
	}
}

void DeathState::Execute(EnemyComponent* Enemy)
{
	
}

void DeathState::OnExit(EnemyComponent* Enemy)
{
}

void DeathState::OnDeathAnimationEnd()
{
	Engine::GetWorld().Destroy(Owner);
}
