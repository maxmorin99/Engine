#include "Object.h"
#include "Engine/Engine.h"
#include "Services/World.h"
#include <sstream>
#include "Components/Component.h"
#include "Components/TransformComponent.h"

size_t Core::Object::sId = -1;
std::string Core::Object::sName = "Object_";

Core::Object::Object()
{
    // Generate Id
    size_t Id = GenerateId();
    std::ostringstream Oss;
    Oss << Id;
    std::string IdStr = Oss.str();
    mId = sName + IdStr;

    // Instantiate components
    mTransform = AddComponent<TransformComponent>();
}

size_t Core::Object::GenerateId()
{
    return ++sId;
}

void Core::Object::GetLocation(float* OutX, float* OutY)
{
    if (!OutX || !OutY) return;
    if (!mTransform)
    {
        *OutX = 0.f;
        *OutY = 0.f;
    }
    
    mTransform->GetLocation(OutX, OutY);
}

void Core::Object::SetLocation(float NewX, float NewY)
{
    if (!mTransform) return;
    mTransform->SetLocation(NewX, NewY);
}

void Core::Object::GetSize(int* OutW, int* OutH) const
{
    if (!OutW || !OutH) return;
    if (!mTransform)
    {
        *OutW = 0;
        *OutH = 0;
        return;
    }
    mTransform->GetSize(OutW, OutH);
}

const Core::Vector<int> Core::Object::GetSize() const
{
    if (!mTransform) return Vector<int>::ZeroVector;
    return mTransform->GetSize();
}

void Core::Object::SetSize(int NewW, int NewH)
{
    if (!mTransform) return;
    mTransform->SetSize(NewW, NewH);
}

void Core::Object::SetSize(const Vector<int>& NewSize)
{
    if (!mTransform) return;
    mTransform->SetSize(NewSize);
}

void Core::Object::SetLocation(const Core::Vector<float>& NewLoc)
{
    if (!mTransform) return;
    mTransform->SetLocation(NewLoc);
}

const Core::Vector<float> Core::Object::GetLocation() const
{
    if (!mTransform) return Vector<float>::ZeroVectorF;
    return mTransform->GetLocation();
}

float Core::Object::GetRotation() const
{
    if (!mTransform) return 0.f;
    return mTransform->GetRotation();
}

void Core::Object::SetRotation(float NewRot)
{
    if (!mTransform) return;
    mTransform->SetRotation(NewRot);
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

    Engine::GetWorld().Destroy(this);
}
