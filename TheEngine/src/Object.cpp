#include "Object.h"
#include "Engine/Engine.h"
#include "Services/World.h"
#include <sstream>
#include "Components/Component.h"

size_t Core::Object::sId = -1;
std::string Core::Object::sName = "Object_";

Core::Object::Object()
{
    size_t Id = GenerateId();
    std::ostringstream Oss;
    Oss << Id;
    std::string IdStr = Oss.str();
    mId = sName + IdStr;
    GetLogger().DebugLog(ConsoleColor::Green, "%s\n", mId.c_str());
    SetId(IdStr);
}

Core::IWorld& Core::Object::GetWorld() const
{
    return Engine::GetWorld();
}

Core::ILogger& Core::Object::GetLogger() const
{
    return Engine::GetLogger();
} 

Core::IGraphic& Core::Object::GetGraphic() const
{
    return Engine::GetGraphic();
}

size_t Core::Object::GenerateId()
{
    return ++sId;
}

Core::IInput& Core::Object::GetInput() const
{
    return Engine::GetInput();
}

void Core::Object::GetLocation(Vector2D<float>& OutLocation) const
{
    OutLocation = mLocation;
}

void Core::Object::Start()
{
    AddComponent<Component>();
    for (Component* Cmp : mComponents)
    {
        Cmp->Start();
    }
    GetComponent<Component>();
}

void Core::Object::Update(float DeltaTime)
{
    for (IUpdatable* Updatable : mUpdatable)
    {
        Updatable->Update(DeltaTime);
    }
}

void Core::Object::Render()
{
    for (IDrawable* Drawable : mDrawable)
    {
        Drawable->Draw();
    }
}

void Core::Object::Destroy()
{
    for (Component* Cmp : mComponents)
    {
        Cmp->Destroy();
        delete Cmp;
    }
    mComponents.clear();
    mDrawable.clear();
    mUpdatable.clear();
    mComponentsByType.clear();

    GetWorld().Destroy(this);
}
