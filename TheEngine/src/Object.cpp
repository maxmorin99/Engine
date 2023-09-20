#include "Object.h"
#include "Engine.h"

Core::World* Core::Object::GetWorld() const
{
    return Engine::GetWorld();
}

Core::IInput* Core::Object::Input() const
{
    return Engine::GetInput();
}
