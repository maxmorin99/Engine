#include "Services/Scene.h"
#include "Object.h"
#include "Engine/Engine.h"
#include "Interfaces/IWorld.h"

Core::Scene::Scene(const char* name) :
	mName(name)
{
}

void Core::Scene::Load()
{
	Object* Player = new Object();
	Player->SetTexturePath("../ArchHero/Assets/Character.png");
	Player->SetSize(100, 100);
	Engine::GetWorld().AddObject(Player);
}
