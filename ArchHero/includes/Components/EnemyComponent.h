#pragma once

#include "Components/Component.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IUpdatable.h"

using namespace Core;

class EnemyComponent : public Component, public IDrawable, public IUpdatable
{
public:
	EnemyComponent(Object* Owner);
	virtual ~EnemyComponent() = default;
	virtual void Start() override;
	virtual void Destroy() override;
	virtual Component* Clone(Object* Owner) override;
	virtual void SetupClone(Component* Child) override;

private:
	PathFindingComponent* mPathFindingComp = nullptr;
	std::vector<Vector<float>> mPath;

	Object* Target = nullptr;

	bool bHasPath = false;

	// Hérité via IDrawable
	virtual void Draw() override;

	// Hérité via IUpdatable
	virtual void Update(float DeltaTime) override;

	void UpdateAnimation();
	void UpdateFlip();
};