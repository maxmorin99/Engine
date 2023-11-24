#include "Components/CollisionComponent.h"
#include "Subject.h"

Core::CollisionComponent::CollisionComponent(Object* InOwner) :
	Component(InOwner),
	mSubjectOnCollisionOverlapBegin(new Subject<std::unordered_map<std::string, void*>>()),
	mSubjectOnCollisionOverlapEnd(new Subject<std::unordered_map<std::string, void*>>()),
	mSubjectOnCollisionHit(new Subject<std::unordered_map<std::string, void*>>()),
	mCollisionChannel(ECollisionChannel::World)
{
	uint8_t ChannelMAX = static_cast<uint8_t>(ECollisionChannel::MAX);
	for (uint8_t i = 0; i < ChannelMAX; i++)
	{
		ECollisionChannel Channel = static_cast<ECollisionChannel>(i);
		mCollisionResponseToChannels[Channel] = ECollisionResponse::Ignore;
	}
}

void Core::CollisionComponent::Start()
{
}

void Core::CollisionComponent::Update(float DeltaTime)
{
}

Core::ECollisionResponse Core::CollisionComponent::GetCollisionResponseToChannel(const ECollisionChannel& Channel)
{
	if (mCollisionResponseToChannels.count(Channel) == 1)
	{
		return mCollisionResponseToChannels[Channel];
	}
	return ECollisionResponse::Ignore;
}

void Core::CollisionComponent::SetCollisionResponseToChannel(const ECollisionChannel& Channel, const ECollisionResponse& Response)
{
	mCollisionResponseToChannels[Channel] = Response;
}

void Core::CollisionComponent::SetCollisionResponseToAllChannels(const ECollisionResponse& Response)
{
	for (uint8_t i = 0; i != static_cast<uint8_t>(ECollisionChannel::MAX); i++)
	{
		SetCollisionResponseToChannel(static_cast<ECollisionChannel>(i), Response);
	}
}

void Core::CollisionComponent::AddOverlappingObject(Object* Obj)
{
	for (int i = 0; i < mOverlappingObjects.size(); i++)
	{
		if (mOverlappingObjects[i] == Obj) return;
	}
	mOverlappingObjects.push_back(Obj);
}

void Core::CollisionComponent::RemoveOverlappingObject(Object* Obj)
{
	for (int i = 0; i < mOverlappingObjects.size(); i++)
	{
		if (Obj == mOverlappingObjects[i])
		{
			mOverlappingObjects.erase(mOverlappingObjects.begin() + i);
		}
	}
}

void Core::CollisionComponent::OnCollisionOverlapBegin(Object* OtherObject, CollisionComponent* OtherComp)
{
	if (!mSubjectOnCollisionOverlapBegin) return;
	std::unordered_map<std::string, void*> Params;
	Params["OtherObject"] = OtherObject;
	Params["OtherComponent"] = OtherComp;
	mSubjectOnCollisionOverlapBegin->Invoke(Params);
}

void Core::CollisionComponent::OnCollisionOverlapEnd(Object* OtherObject, CollisionComponent* OtherComp)
{
	if (!mSubjectOnCollisionOverlapEnd) return;
	std::unordered_map<std::string, void*> Params;
	Params["OtherObject"] = OtherObject;
	Params["OtherComponent"] = OtherComp;
	mSubjectOnCollisionOverlapEnd->Invoke(Params);
}

void Core::CollisionComponent::OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp)
{
	if (!mSubjectOnCollisionHit) return;
	std::unordered_map<std::string, void*> Params;
	Params["OtherObject"] = OtherObject;
	Params["OtherComponent"] = OtherComp;
	mSubjectOnCollisionHit->Invoke(Params);
}

bool Core::CollisionComponent::IsOverlappingWith(Object* OtherObject) const
{
	for (Object* Obj : mOverlappingObjects)
	{
		if (Obj == OtherObject) return true;
	}
	
	return false;
}

void Core::CollisionComponent::BindOnCollisionOverlapBegin(IObserver<std::unordered_map<std::string, void*>>* O)
{
	if (!mSubjectOnCollisionOverlapBegin) return;
	mSubjectOnCollisionOverlapBegin->AddListener(O);
}

void Core::CollisionComponent::BindOnCollisionOverlapEnd(IObserver<std::unordered_map<std::string, void*>>* O)
{
	if (!mSubjectOnCollisionOverlapEnd) return;
	mSubjectOnCollisionOverlapEnd->AddListener(O);
}

void Core::CollisionComponent::BindOnCollisionHit(IObserver<std::unordered_map<std::string, void*>>* O)
{
	if (!mSubjectOnCollisionHit) return;
	mSubjectOnCollisionHit->AddListener(O);
}

void Core::CollisionComponent::UnBindOnCollisionOverlapBegin(IObserver<std::unordered_map<std::string, void*>>* O)
{
	if (!mSubjectOnCollisionOverlapBegin) return;
	mSubjectOnCollisionOverlapBegin->RemoveListener(O);
}

void Core::CollisionComponent::UnBindOnCollisionOverlapEnd(IObserver<std::unordered_map<std::string, void*>>* O)
{
	if (!mSubjectOnCollisionOverlapEnd) return;
	mSubjectOnCollisionOverlapEnd->RemoveListener(O);
}

void Core::CollisionComponent::UnBindOnCollisionHit(IObserver<std::unordered_map<std::string, void*>>* O)
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

void Core::CollisionComponent::SetCollisionLocation(const Vector<float>& NewLoc)
{
}

Core::Vector<float> Core::CollisionComponent::GetCollisionLocation() const
{
	return Vector<float>();
}
