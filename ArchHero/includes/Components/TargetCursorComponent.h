#pragma once

#include "Components/SpriteComponent.h"

using namespace Core;

class TargetCursorComponent : public SpriteComponent, public IUpdatable
{
public:
	TargetCursorComponent(Object* Owner);
	virtual void Update(float DeltaTime) override;
	virtual void Draw() override;
	virtual Component* Clone(Object* Owner) override;
	virtual void SetupClone(Component* Child) override;
};