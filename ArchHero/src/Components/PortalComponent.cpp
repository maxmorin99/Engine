#include "Components/PortalComponent.h"
#include "Components/CollisionComponent.h"
#include "Object.h"

PortalComponent::PortalComponent(Core::Object* Owner) :
	Component(Owner)
{
}

void PortalComponent::Start()
{
	Core::CollisionComponent* ColComp = mOwner->GetCollisionComponent();
	if (ColComp)
	{
		ColComp->BindOnCollisionOverlapBegin(this);
	}
}

void PortalComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	void* OtherObject = nullptr;

	if (Value.count("OtherObject") == 0) return;

	OtherObject = Value.at("OtherObject");
	if (!OtherObject) return;

	Core::Object* Other = static_cast<Core::Object*>(OtherObject);
	if (Other && Other->HasTag("Player"))
	{
		World().Load("FirstScene");
	}
}
