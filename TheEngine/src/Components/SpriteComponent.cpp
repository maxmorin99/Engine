#include "Components/SpriteComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IGraphic.h"
#include "Object.h"

Core::SpriteComponent::SpriteComponent(Object* Owner) :
	Component(Owner)
{
}

Core::SpriteComponent::SpriteComponent(Object* Owner, const std::string& File) :
	Component(Owner),
	mFile(File)
{
}

void Core::SpriteComponent::Start()
{
	mTextureId = Engine::GetGraphic().LoadTexture(mFile.c_str());
}

void Core::SpriteComponent::Destroy()
{
}

void Core::SpriteComponent::Draw()
{
	if (!mOwner) return;

	Vector<float> OwnerSize = mOwner->GetSize();
	Vector<float> OwnerLoc = mOwner->GetLocation();
	Rect<int> Dst(static_cast<int>(OwnerLoc.X), static_cast<int>(OwnerLoc.Y), static_cast<int>(OwnerSize.X), static_cast<int>(OwnerSize.Y));
	
	Engine::GetGraphic().SetDrawColor(mColor);
	Engine::GetGraphic().DrawTexture(mTextureId, Dst, mColor);
}
