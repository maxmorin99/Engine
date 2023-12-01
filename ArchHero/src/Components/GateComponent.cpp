#include "Components/GateComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/AttributeComponent.h"
#include "Object.h"
#include <vector>

GateComponent::GateComponent(Object* Owner) :
	Component(Owner)
{
}

Component* GateComponent::Clone(Object* Owner)
{
	GateComponent* Clone = new GateComponent(Owner);
	__super::SetupClone(Clone);
	Clone->mSoundId = mSoundId;

	return Clone;
}

void GateComponent::SetupClone(Component* Child)
{
	GateComponent* Clone = dynamic_cast<GateComponent*>(Child);
	__super::SetupClone(Clone);
	Clone->mSoundId = mSoundId;
}

void GateComponent::Start()
{
	if (!mOwner) return;

	std::vector<Object*> Enemies = World().GetObjectsWithTag("Enemy");
	SetEnemiesCount(Enemies.size());
	for (Object* o : Enemies)
	{
		if (AttributeComponent* EnemyAttributeComp = o->GetComponent<AttributeComponent>())
		{
			EnemyAttributeComp->mOnDeathSubject.AddListener(this);
		}
	}

	// adjust box
	BoxComponent* Box = mOwner->GetComponent<BoxComponent>();
	if (Box)
	{
		Box->SetBoxSize(mOwner->GetSize().X * 0.5f, mOwner->GetSize().Y * 0.5f);
		Box->SetOffset(mOwner->GetSize().X * 0.27f, mOwner->GetSize().Y * 0.4f);

		// adjust position
		mOwner->SetLocation(mOwner->GetLocation() + Box->GetCollisionLocation());
	}

}

void GateComponent::OnOpenAnimationEnd()
{
	if (!mOwner) return;
	if (AnimationComponent* AnimComp = mOwner->GetComponent<AnimationComponent>())
	{
		AnimComp->Disable();
	}
	if (BoxComponent* BoxComp = mOwner->GetComponent<BoxComponent>())
	{
		BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);
	}
}

void GateComponent::SetAudioFile(const std::string& File)
{
	mSoundId = Audio().LoadSound(File.c_str());
}

void GateComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	mEnemiesCount--;
	if (mEnemiesCount == 0)
	{
		if (mOwner && mOwner->GetComponent<AnimationComponent>())
		{
			mOwner->GetComponent<AnimationComponent>()->SetClip("Open", false, std::bind(&GateComponent::OnOpenAnimationEnd, this));
			Audio().PlaySFX(mSoundId);
		}
	}
}
