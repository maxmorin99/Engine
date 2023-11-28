#pragma once
#include "Engine/Core.h"
#include "Interfaces/IState.h"

class DeathState : public IState
{
public:
	DeathState();
	virtual ~DeathState() = default;
	void OnEnter(EnemyComponent* Enemy) override;
	void Execute(EnemyComponent* Enemy) override;
	void OnExit(EnemyComponent* Enemy) override;

private:
	Core::Object* Owner = nullptr;
	void OnDeathAnimationEnd();
};