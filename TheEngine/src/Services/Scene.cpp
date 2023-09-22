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
	Object* Obj1 = new Object();
	Object* Obj2 = new Object();

	Engine::GetWorld().AddObject(Obj1);
	Engine::GetWorld().AddObject(Obj2);
}
