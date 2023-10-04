#include "Services/Scene.h"
#include "Object.h"
#include "Engine/Engine.h"
#include "Interfaces/IWorld.h"
#include "Components/AtlasComponent.h"
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
	Player->SetSize(250, 250);
	Player->AddComponent<PlayerComponent>();

	AtlasComponent* Atlas = Player->AddComponent<AtlasComponent>();
	std::string FilePath = ASSET_PATH + std::string("Character_Atlas.png");
	Atlas->SetFile(FilePath);
	
	PhysicComponent* Px = Player->AddComponent<PhysicComponent>();
	Px->SetMass(1.f);
	Px->SetSlideFactor(0);
	Px->SetMaxMovementSpeed(250.f);
	Px->SetAccelerationSpeed(10000);
	Px->SetDecelerationSpeed(10000);


	Engine::GetWorld().AddObject(Player);
}
