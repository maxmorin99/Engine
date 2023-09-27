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
    mTextureId = GetGraphic().LoadTexture(mTexturePath.c_str());
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
    int InTextureW = 0;
    int InTextureH = 0;
    GetGraphic().GetTextureSize(mTextureId, &InTextureW, &InTextureH);
    int Idx = 2;
    Rect<int> Src =  {
        0,
        115,
        InTextureW / 8,
        InTextureH / 4
    };

    Rect<int> Dst = {
        100,
        100,
        mW,
        mH
    };
    
    GetGraphic().DrawTexture(mTextureId, Src, Dst, 0, Flip::None, Color::White);
}

void Core::Object::Destroy()
{
    GetWorld().Destroy(this);
}
