#include "Components/CollisionComponent.h"

Core::CollisionComponent::CollisionComponent(Object* InOwner) :
	Component(InOwner)
{
	uint8_t ChannelMAX = static_cast<uint8_t>(ECollisionChannel::MAX);
	for (uint8_t i = 0; i < ChannelMAX; i++)
	{
		ECollisionChannel Channel = static_cast<ECollisionChannel>(i);
		mCollisionResponseToChannels[Channel] = ECollisionResponse::Ignore;
	}
}

Core::ECollisionResponse Core::CollisionComponent::GetCollisionResponseToChannel(const ECollisionChannel& Channel)
{
	if (mCollisionResponseToChannels.count(Channel) == 1)
	{
		return mCollisionResponseToChannels[Channel];
	}
	return ECollisionResponse::Ignore;
}

void Core::CollisionComponent::AddCollisionResponseToChannel(const ECollisionChannel& Channel, const ECollisionResponse& Response)
{
	mCollisionResponseToChannels[Channel] = Response;
}

void Core::CollisionComponent::AddOverlappingObjects(Object* Obj)
{
	for (int i = 0; i < mOverlappingObjects.size(); i++)
	{
		if (mOverlappingObjects[i] == Obj) return;
	}
	mOverlappingObjects.push_back(Obj);
}

void Core::CollisionComponent::Destroy()
{
	Component::Destroy();

	mCollisionResponseToChannels.clear();
	mOverlappingObjects.clear();
}
