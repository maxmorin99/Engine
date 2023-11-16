#pragma once

#include "Components/Component.h"
#include "Interfaces/IDrawable.h"

using namespace Core;

class EnemyComponent : public Component, public IDrawable
{
public:
	EnemyComponent(Object* Owner);
	virtual ~EnemyComponent() = default;
	virtual void Start() override;
	virtual void Destroy() override;

private:
	PathFindingComponent* mPathFindingComp = nullptr;

	// Hérité via IDrawable
	virtual void Draw() override;
};