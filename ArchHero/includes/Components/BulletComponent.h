#pragma once

#include "Components/SpriteComponent.h"
#include "Interfaces/IUpdatable.h"

using namespace Core;

class BulletComponent : public SpriteComponent, public IUpdatable
{
public:
	BulletComponent(Object* Owner);
	virtual void Update(float DeltaTime) override;

private:
	
};