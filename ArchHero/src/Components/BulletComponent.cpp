#include "Components/BulletComponent.h"
#include "Object.h"
#include "Components/PhysicComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxComponent.h"

BulletComponent::BulletComponent(Core::Object* Owner) :
	SpriteComponent(Owner)
{
}

void BulletComponent::Start()
{
	SpriteComponent::Start();

	mPxComp = mOwner->GetComponent<Core::PhysicComponent>();
	Core::CollisionComponent* ColComp = mOwner->GetCollisionComponent();
	if (ColComp)
	{
		ColComp->BindOnCollisionOverlapBegin(this);
	}
}

void BulletComponent::Update(float DeltaTime)
{
	// Move by fwd dir
	if (!mPxComp || !bShouldMove) return;
	mPxComp->AddMovement(mOwner->GetForwardVector().GetNormalized());
}

void BulletComponent::Destroy()
{
	SpriteComponent::Destroy();
}

void BulletComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	void* OtherObject = nullptr;
	void* OtherComponent = nullptr;

	if (Value.count("OtherObject") > 0)
	{
		OtherObject = Value.at("OtherObject");
	}
	if (Value.count("OtherComponent") > 0)
	{
		OtherComponent = Value.at("OtherComponent");
	}

	if (OtherObject)
	{
		if (mOwner)
		{
			bShouldMove = false;
			if (AnimationComponent* AnimComp = mOwner->GetComponent<AnimationComponent>())
			{

				if (BoxComponent* BoxComp = mOwner->GetComponent<BoxComponent>())
				{
					BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);
				}
			}
		}
	}
}

void BulletComponent::OnExplosionEnd()
{
	World().Destroy(mOwner);
}
