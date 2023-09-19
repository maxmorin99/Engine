#include "World.h"
#include "ObjectFactory.h"
#include <sstream>

Core::World::World()
{
}

Core::World::~World()
{
}

void Core::World::Update(float DetaTime)
{
}

void Core::World::Draw()
{
}

void Core::World::SetObjectId(Object* InObj) const
{
	std::ostringstream Oss;
	Oss << InObj;
	std::string IdStr = Oss.str();
	size_t ObjectId = std::hash<std::string>()(IdStr.c_str());
	InObj->SetId(ObjectId);
	InObj->Init();
}
