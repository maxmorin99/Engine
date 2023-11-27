#pragma once

#include "Components/SpriteComponent.h"
#include "Interfaces/IUpdatable.h"
#include "Interfaces/IObserver.h"

using namespace Core;

class BulletComponent : public SpriteComponent, public IUpdatable, public IObserver<std::unordered_map<std::string, void*>>
{
public:
	BulletComponent(Object* Owner);
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;
	virtual Component* Clone(Object* Owner) override;
	virtual void SetupClone(Component* Child) override;

	virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

private:
	PhysicComponent* mPxComp = nullptr;

	void OnExplosionEnd();
};


