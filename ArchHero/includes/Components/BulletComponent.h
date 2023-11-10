#pragma once

#include "Components/SpriteComponent.h"
#include "Interfaces/IUpdatable.h"
#include "Interfaces/IObserver.h"


class Core::PhysicComponent;

class BulletComponent : public Core::SpriteComponent, public Core::IUpdatable, public Core::IObserver<std::unordered_map<std::string, void*>>
{
public:
	BulletComponent(Core::Object* Owner);
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;

	virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

private:
	Core::PhysicComponent* mPxComp = nullptr;
};


