#pragma once

#include "Interfaces/IState.h"

class AttackState : public IState
{
public:
	AttackState();
	virtual ~AttackState() = default;
	void OnEnter(EnemyComponent* Enemy) override;
	void Execute(EnemyComponent* Enemy) override;
	void OnExit(EnemyComponent* Enemy) override;
};