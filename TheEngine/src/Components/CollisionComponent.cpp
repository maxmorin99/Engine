#include "Components/CollisionComponent.h"
#include "Subject.h"

Core::CollisionComponent::CollisionComponent(Object* InOwner) :
	Component(InOwner),
	mSubjectOnCollisionOverlapBegin(new SubjectTwoParams<Object*, CollisionComponent*>()),
	mSubjectOnCollisionOverlapEnd(new SubjectTwoParams<Object*, CollisionComponent*>()),
	mSubjectOnCollisionHit(new SubjectTwoParams<Object*, CollisionComponent*>()),
	mCollisionChannel(ECollisionChannel::World)
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

void Core::CollisionComponent::AddOverlappingObject(Object* Obj)
{
	for (int i = 0; i < mOverlappingObjects.size(); i++)
	{
		if (mOverlappingObjects[i] == Obj) return;
	}
	mOverlappingObjects.push_back(Obj);
}

void Core::CollisionComponent::OnCollisionOverlapBegin(Object* OtherObject, CollisionComponent* OtherComp)
{
	if (!mSubjectOnCollisionOverlapBegin) return;
	mSubjectOnCollisionOverlapBegin->Invoke(OtherObject, OtherComp);
}

void Core::CollisionComponent::OnCollisionOverlapEnd(Object* OtherObject, CollisionComponent* OtherComp)
{
	if (!mSubjectOnCollisionOverlapEnd) return;
	mSubjectOnCollisionOverlapEnd->Invoke(OtherObject, OtherComp);
}

void Core::CollisionComponent::OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp)
{
	if (!mSubjectOnCollisionHit) return;
	mSubjectOnCollisionHit->Invoke(OtherObject, OtherComp);
}

bool Core::CollisionComponent::IsOverlappingWith(Object* OtherObject) const
{
	for (Object* Obj : mOverlappingObjects)
	{
		if (Obj == OtherObject) return true;
	}
	return false;
}

void Core::CollisionComponent::BindOnCollisionOverlapBegin(IObserverTwoParams<Object*, CollisionComponent*>* O)
{
	if (!mSubjectOnCollisionOverlapBegin) return;
	mSubjectOnCollisionOverlapBegin->AddListener(O);
}

void Core::CollisionComponent::BindOnCollisionOverlapEnd(IObserverTwoParams<Object*, CollisionComponent*>* O)
{
	if (!mSubjectOnCollisionOverlapEnd) return;
	mSubjectOnCollisionOverlapEnd->AddListener(O);
}

void Core::CollisionComponent::BindOnCollisionHit(IObserverTwoParams<Object*, CollisionComponent*>* O)
{
	if (!mSubjectOnCollisionHit) return;
	mSubjectOnCollisionHit->AddListener(O);
}

void Core::CollisionComponent::UnBindOnCollisionOverlapBegin(IObserverTwoParams<Object*, CollisionComponent*>* O)
{
	if (!mSubjectOnCollisionOverlapBegin) return;
	mSubjectOnCollisionOverlapBegin->RemoveListener(O);
}

void Core::CollisionComponent::UnBindOnCollisionOverlapEnd(IObserverTwoParams<Object*, CollisionComponent*>* O)
{
	if (!mSubjectOnCollisionOverlapEnd) return;
	mSubjectOnCollisionOverlapEnd->RemoveListener(O);
}

void Core::CollisionComponent::UnBindOnCollisionHit(IObserverTwoParams<Object*, CollisionComponent*>* O)
{
	if (!mSubjectOnCollisionHit) return;
	mSubjectOnCollisionHit->RemoveListener(O);
}

void Core::CollisionComponent::Destroy()
{
	Component::Destroy();

	mCollisionResponseToChannels.clear();
	mOverlappingObjects.clear();

	delete mSubjectOnCollisionOverlapBegin;
	delete mSubjectOnCollisionOverlapEnd;
	delete mSubjectOnCollisionHit;
}