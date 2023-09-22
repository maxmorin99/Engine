#include "Object.h"
#include "Engine/Engine.h"
#include "Services/World.h"
#include <sstream>

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

void Core::Object::GetSize(float* OutW, float* OutH) const
{
    if (!OutW || !OutH) return;
    *OutW = mW;
    *OutH = mH;
}

void Core::Object::Start()
{
    GetLogger().DebugLog(ConsoleColor::Red, "Start!\n");
}

void Core::Object::Update(float deltaTime)
{
    if (GetInput().IsKeyDown(EKey::Down))
    {
        GetLogger().DebugLog(ConsoleColor::Red, "Down key pressed!\n");
    }
}

void Core::Object::Render()
{
}

void Core::Object::Destroy()
{
    GetWorld().Destroy(this);
}