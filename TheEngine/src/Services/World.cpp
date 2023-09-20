#include "Services/World.h"
#include <sstream>
#include "Object.h"

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
	InObj->SetId(IdStr);
}
