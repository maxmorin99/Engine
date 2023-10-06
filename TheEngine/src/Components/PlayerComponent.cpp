#include "Components/PlayerComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IInput.h"
#include "Object.h"
#include "Components/PhysicComponent.h"
#include "Components/AtlasComponent.h"

Core::PlayerComponent::PlayerComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PlayerComponent::Start()
{
}

void Core::PlayerComponent::Update(float DeltaTime)
{
	if (!mOwner) return;
	Vector<float> MovementInput = Vector<float>::ZeroVector();

	if (Input().IsKeyDown(EKey::D))
	{
		MovementInput.X += 1.f;
	}
	if (Input().IsKeyDown(EKey::A))
	{
		MovementInput.X -= 1.f;
	}
	if (Input().IsKeyDown(EKey::W))
	{
		MovementInput.Y += 1.f;
	}
	if (Input().IsKeyDown(EKey::S))
	{
		MovementInput.Y -= 1.f;
	}

	PhysicComponent* PxComp = mOwner->GetComponent<PhysicComponent>();
	if (PxComp)
	{
		PxComp->AddMovement(MovementInput);
	}

	if (Input().IsKeyDown(EKey::Space))
	{
		AtlasComponent* Atlas = mOwner->GetComponent<AtlasComponent>();
		if (Atlas)
		{
			Atlas->SetFrame("Idle2");
		}
	}
}

void Core::PlayerComponent::Destroy()
{
}
