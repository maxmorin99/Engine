#include "Services/Scene.h"
#include "Object.h"
#include "Engine/Engine.h"
#include "Interfaces/IWorld.h"
#include "Components/SpriteComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/PhysicComponent.h"
#include "Interfaces/IAudio.h"

Core::Scene::Scene(const char* name) :
	mName(name)
{
}

void Core::Scene::Load()
{
	Object* Player = new Object();
	Player->SetLocation(100.f, 100.f);
	Player->SetSize(100, 100);
	SpriteComponent* Sprite = Player->AddComponent<SpriteComponent>();
	Sprite->SetFile(ASSET_PATH + std::string("Character_Atlas.png"));
	Player->AddComponent<PlayerComponent>();
	PhysicComponent* Px = Player->AddComponent<PhysicComponent>();
	Px->SetMass(1.f);
	Px->SetMaxMovementSpeed(250.f);
	Px->SetMovementSpeed(10.f);

	


	Engine::GetWorld().AddObject(Player);
}
