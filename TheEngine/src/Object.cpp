#include "Object.h"
#include "Engine/Engine.h"
#include "Services/World.h"

Core::World* Core::Object::GetWorld() const
{
    return Engine::GetWorld();
}

Core::ILogger* Core::Object::GetLogger() const
{
    return Engine::GetLogger();
}

Core::IInput* Core::Object::GetInput() const
{
    
    return Engine::GetInput();
}

void Core::Object::Destroy()
{
    GetWorld()->Destroy(this);
}
