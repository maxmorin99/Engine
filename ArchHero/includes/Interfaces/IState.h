#pragma once

class EnemyComponent;

class IState
{
public:
	virtual ~IState() = default;
	virtual void OnEnter(EnemyComponent* Enemy) = 0;
	virtual void Execute(EnemyComponent* Enemy) = 0;
	virtual void OnExit(EnemyComponent* Enemy) = 0;
};