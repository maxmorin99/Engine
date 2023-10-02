#include "Components/PlayerComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IInput.h"
#include "Object.h"

Core::PlayerComponent::PlayerComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PlayerComponent::Start()
{
}

void Core::PlayerComponent::Update(float DeltaTime)
{
	if (Engine::GetInput().IsKeyDown(EKey::D))
	{
		Vector<float> CurrLoc = mOwner->GetLocation();
		CurrLoc.X += 100.f * DeltaTime;
		mOwner->SetLocation(CurrLoc);
	}
}

void Core::PlayerComponent::Destroy()
{
}
