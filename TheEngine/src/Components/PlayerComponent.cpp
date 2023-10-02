#include "Components/PlayerComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/IInput.h"
#include "Object.h"
#include "Components/PhysicComponent.h"

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
	if (Engine::GetInput().IsKeyDown(EKey::D))
	{
		PhysicComponent* PxComp = mOwner->GetComponent<PhysicComponent>();
		if (PxComp)
		{
			PxComp->AddMovement(Vector<float>(1.f, 0));
		}
	}
}

void Core::PlayerComponent::Destroy()
{
}
