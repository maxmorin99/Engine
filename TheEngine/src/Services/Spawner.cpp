#include "Services/Spawner.h"
#include "Object.h"

Core::Spawner::Spawner() :
	mProtoMap(new std::unordered_map<std::string, Object*>())
{
}

void Core::Spawner::AddPrototype(const std::string& name, Object* Obj)
{
	if (!Obj) return;

	mProtoMap->emplace(name, Obj);
}

Core::Object* Core::Spawner::Spawn(const std::string& name)
{
	if (mProtoMap->count(name) == 0) return nullptr;
	return (*mProtoMap)[name]->Clone();
}

void Core::Spawner::Shutdown()
{
	for (auto& pair : *mProtoMap)
	{
		pair.second->Destroy();
		delete pair.second;
	}
	mProtoMap->clear();
	delete mProtoMap;
}
