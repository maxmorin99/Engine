#include "Components/BulletComponent.h"
#include "Object.h"
#include "Components/PhysicComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"


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
	Clone->mDamage = mDamage;
	Clone->mImpactWallSoundIdList = mImpactWallSoundIdList;
	Clone->mImpactBodySoundIdList = mImpactBodySoundIdList;

	return Clone;
}

void BulletComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);

	BulletComponent* Clone = dynamic_cast<BulletComponent*>(Child);
	if (Clone)
	{
		Clone->mDamage = mDamage;
		Clone->mImpactWallSoundIdList = mImpactWallSoundIdList;
		Clone->mImpactBodySoundIdList = mImpactBodySoundIdList;
	}
}

void BulletComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	void* OtherObjectVoidPtr = nullptr;
	void* OtherComponentVoidPtr = nullptr;

	if (Value.count("OtherObject") > 0)
	{
		OtherObjectVoidPtr = Value.at("OtherObject");
	}
	if (Value.count("OtherComponent") > 0)
	{
		OtherComponentVoidPtr = Value.at("OtherComponent");
	}

	if (OtherObjectVoidPtr)
	{
		DisableCollision();

		// Disable pxComp to stop moving
		DisablePhysic();

		Object* OtherObject = static_cast<Object*>(OtherObjectVoidPtr);
		if (OtherObject)
		{
			if (OtherObject->HasTag("Character"))
			{
				// Apply Damage
				AttributeComponent* Attribute = OtherObject->GetComponent<AttributeComponent>();
				if (Attribute)
				{
					Attribute->TakeDamage(mDamage);
				}

				// play random body impact sound
				int RandomId = std::rand() % mImpactBodySoundIdList.size();
				Audio().PlaySFX(mImpactBodySoundIdList[RandomId], false);

				// Play blood vfx
				PlayDestroyAnimation("BloodClip");
			}
			else
			{
				// play random impact sound
				int RandomId = std::rand() % mImpactWallSoundIdList.size();
				Audio().PlaySFX(mImpactWallSoundIdList[RandomId], false);

				// Play explosion vfx
				PlayDestroyAnimation("ExplosionClip");
			}
		}
	}
}

void BulletComponent::AddImpactWallSoundId(const std::vector<size_t>& InId)
{
	mImpactWallSoundIdList = InId;
}

void BulletComponent::AddImpactBodySoundId(const std::vector<size_t>& InId)
{
	mImpactBodySoundIdList = InId;
}

void BulletComponent::DisableCollision()
{
	if (!mOwner) return;
	if (BoxComponent* BoxComp = mOwner->GetComponent<BoxComponent>())
	{
		BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);
	}
}

void BulletComponent::DisablePhysic()
{
	if (!mOwner) return;
	if (mPxComp)
	{
		mPxComp->Disable();
	}
}

void BulletComponent::PlayDestroyAnimation(const std::string& AnimationName)
{
	if (!mOwner) return;
	AnimationComponent* AnimComp = mOwner->GetComponent<AnimationComponent>();
	if (AnimComp)
	{
		AnimComp->SetClip(AnimationName, false, std::bind(&BulletComponent::OnExplosionEnd, this));
		mOwner->SetSize(mOwner->GetSize().X * 3, mOwner->GetSize().Y * 3);
		mOwner->SetLocation(mOwner->GetLocation().X - mOwner->GetSize().X / 3, mOwner->GetLocation().Y - mOwner->GetSize().Y / 3);
	}
}

void BulletComponent::OnExplosionEnd()
{
	World().Destroy(mOwner);
}
