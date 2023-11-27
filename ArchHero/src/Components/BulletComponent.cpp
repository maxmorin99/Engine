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
	if (!mPxComp) return;
	mPxComp->AddMovement(mOwner->GetForwardVector().GetNormalized());
}

void BulletComponent::Destroy()
{
	SpriteComponent::Destroy();
}

Component* BulletComponent::Clone(Object* Owner)
{
	BulletComponent* Clone = new BulletComponent(Owner);
	__super::SetupClone(Clone);

	return Clone;
}

void BulletComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);
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
			// Disable pxComp to stop moving
			if (mPxComp)
			{
				mPxComp->Disable();
			}

			if (AnimationComponent* AnimComp = mOwner->GetComponent<AnimationComponent>())
			{
				AnimComp->SetClip("ExplosionClip", false, std::bind(&BulletComponent::OnExplosionEnd, this));
				mOwner->SetSize(mOwner->GetSize().X * 3, mOwner->GetSize().Y * 3);
				mOwner->SetLocation(mOwner->GetLocation().X - mOwner->GetSize().X / 3, mOwner->GetLocation().Y - mOwner->GetSize().Y / 3);

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
