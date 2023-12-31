#include "Object.h"
#include "Engine/Engine.h"
#include "Services/World.h"
#include <sstream>
#include "Components/TransformComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxComponent.h"

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

Core::Vector<float> Core::Object::GetCenterLocation() const
{
    Vector<float> Loc = GetLocation();
    Vector<float> Size = GetSize();
    Vector<float> SpriteCenterOffset = GetCenterOffset();
    return Vector<float>(Loc.X + Size.X / 2 + SpriteCenterOffset.X, Loc.Y + Size.Y / 2 + SpriteCenterOffset.Y);
}

Core::Vector<float> Core::Object::GetSpriteCenterLocation() const
{
    Vector<float> Size = GetSize();
    Vector<float> Loc = GetLocation();
    Vector<float> CenterOffset = GetCenterOffset();
    return Vector<float>(Loc.X + Size.X / 2 + CenterOffset.X, Loc.Y + Size.Y / 2 + CenterOffset.Y);
}

void Core::Object::SetLocation(float NewX, float NewY)
{
    if (!mTransform) return;
    mTransform->SetLocation(NewX, NewY);
}

void Core::Object::SetCenterLocation(const Vector<float>& NewLoc)
{
    if (!mTransform) return;
    Vector<float> CenterLoc = NewLoc;
    CenterLoc.X -= GetSize().X / 2;
    CenterLoc.Y -= GetSize().Y / 2;
    mTransform->SetLocation(CenterLoc);
}

void Core::Object::SetImgTopLeftLocation(const Vector<float>& NewLoc)
{
    if (!mTransform) return;
    BoxComponent* Box = GetComponent<BoxComponent>();
    if (Box)
    {
        Vector<float> BoxOffset = Box->GetOffset();
        mTransform->SetLocation(Vector<float>(NewLoc.X - BoxOffset.X, NewLoc.Y - BoxOffset.Y));
    }
}

void Core::Object::GetSize(float* OutW, float* OutH) const
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

const Core::Vector<float> Core::Object::GetSize() const
{
    if (!mTransform) return Vector<float>::ZeroVector();
    return mTransform->GetSize();
}

void Core::Object::SetSize(float NewW, float NewH)
{
    if (!mTransform) return;
    mTransform->SetSize(NewW, NewH);
}

void Core::Object::SetSize(const Vector<float>& NewSize)
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
    if (!mTransform) return Vector<float>::ZeroVector();
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

Core::Vector<float> Core::Object::GetForwardVector() const
{
    if (!mTransform) return Vector<float>::ZeroVector();
    return mTransform->GetForwardVector();
}

Core::Vector<float> Core::Object::GetVelocity() const
{
    PhysicComponent* PxComp = GetComponent<PhysicComponent>();
    if (!PxComp) return Vector<float>::ZeroVector();
    return PxComp->GetVelocity();
}

void Core::Object::SetFlip(const Flip& InFlip)
{
    AnimationComponent* AnimComp = GetComponent<AnimationComponent>();
    if (AnimComp)
    {
        AnimComp->SetFlip(InFlip);
        return;
    }
    AtlasComponent* AtlasComp = GetComponent<AtlasComponent>();
    if (AtlasComp)
    {
        AtlasComp->SetFlip(InFlip);
        return;
    }
    SpriteComponent* SpriteComp = GetComponent<SpriteComponent>();
    if (SpriteComp)
    {
        SpriteComp->SetFlip(InFlip);
    }
}

Core::Flip Core::Object::GetFlip() const
{
    AnimationComponent* AnimComp = GetComponent<AnimationComponent>();
    if (AnimComp)
    {
        return AnimComp->GetFlip();
    }
    AtlasComponent* AtlasComp = GetComponent<AtlasComponent>();
    if (AtlasComp)
    {
        return AtlasComp->GetFlip();
    }
    SpriteComponent* SpriteComp = GetComponent<SpriteComponent>();
    if (SpriteComp)
    {
        return SpriteComp->GetFlip();
    }
    return Flip::None;
}

void Core::Object::AddTag(const std::string& Tag)
{
    if (!HasTag(Tag))
    {
        mTags.push_back(Tag);
    }
}

bool Core::Object::HasTag(const std::string& Tag) const
{
    for (int i = 0; i < mTags.size(); i++)
    {
        if (mTags[i] == Tag) return true;
    }
    return false;
}

void Core::Object::Start()
{
    for (Component* Cmp : mComponents)
    {
        Cmp->Start();
    }

    bStarted = true;
}

void Core::Object::Update(float DeltaTime)
{
    if (mTransform)
    {
        mTransform->SetOldLocation(mTransform->GetLocation());
    }
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
}

void Core::Object::UseOldLocation()
{
    if (!mTransform) return;
    Vector<float> OldLoc = mTransform->GetOldLocation();
    mTransform->SetLocation(OldLoc);
}

Core::Object* Core::Object::Clone()
{
    Object* Clone = new Object();
    Clone->mTags = mTags;
    
    for (Component* c : mComponents)
    {
        // Only get the size of the transform since each Object has a default transform component
        if (TransformComponent* t = dynamic_cast<TransformComponent*>(c))
        {
            Clone->SetSize(t->GetSize());
            continue;
        }
        Component* ClonedComp = c->Clone(Clone);
        Clone->AddComponent(ClonedComp);
    }
    return Clone;
}

void Core::Object::AddComponent(Component* comp)
{
    if (!comp) return;

    mComponents.push_back(comp);
    const type_info* Type = &typeid(*comp);
    mComponentsByType.emplace(Type, comp);

    IUpdatable* Updatable = dynamic_cast<IUpdatable*>(comp);
    if (Updatable)
    {
        mUpdatable.push_back(Updatable);
    }
    IDrawable* Drawable = dynamic_cast<IDrawable*>(comp);
    if (Drawable)
    {
        mDrawable.push_back(Drawable);
    }
}

Core::CollisionComponent* Core::Object::GetCollisionComponent() const
{
    BoxComponent* BoxComp = GetComponent<BoxComponent>();
    if (BoxComp)
    {
        return BoxComp;
    }
    // TODO make the same logic with circle
    return nullptr;
}

Core::Vector<float> Core::Object::GetCenterOffset() const
{
    SpriteComponent* SC = GetComponent<SpriteComponent>();
    if (SC)
    {
        return SC->GetCenterOffset();
    }
    AtlasComponent* AC = GetComponent<AtlasComponent>();
    if (AC)
    {
        return AC->GetCenterOffset();
    }
    AnimationComponent* AnimC = GetComponent<AnimationComponent>();
    if (AnimC)
    {
        return AnimC->GetCenterOffset();
    }
    return Vector<float>::ZeroVector();
}
