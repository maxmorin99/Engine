#pragma once

#include "Interfaces/IState.h"

class ChaseState : public IState
{
public:
	ChaseState();
	virtual ~ChaseState() = default;
	void OnEnter(EnemyComponent* Enemy) override;
	void Execute(EnemyComponent* Enemy) override;
	void OnExit(EnemyComponent* Enemy) override;
};