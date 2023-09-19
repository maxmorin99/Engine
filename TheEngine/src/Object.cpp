#include "Object.h"
#include "Engine.h"

Core::World* Core::Object::GetWorld() const
{
    return Engine::GetWorld();
}
