#include "Components/ChangeSceneTrigger.h"
#include "Components/BoxComponent.h"
#include "Object.h"

ChangeSceneTrigger::ChangeSceneTrigger(Object* Owner) :
	Component(Owner)
{
}

void ChangeSceneTrigger::Start()
{
	__super::Start();
	if (!mOwner) return;
	if (BoxComponent* Box = mOwner->GetComponent<BoxComponent>())
	{
		Box->BindOnCollisionOverlapBegin(this);
		Box->SetBoxSize(mOwner->GetSize().X, mOwner->GetSize().Y);
	}
}

Component* ChangeSceneTrigger::Clone(Object* Owner)
{
	ChangeSceneTrigger* Clone = new ChangeSceneTrigger(Owner);
	__super::SetupClone(Clone);

	return Clone;
}

void ChangeSceneTrigger::SetupClone(Component* Child)
{
	ChangeSceneTrigger* Clone = dynamic_cast<ChangeSceneTrigger*>(Child);
	if (!Clone) return;
	__super::SetupClone(Clone);
}

void ChangeSceneTrigger::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	void* OtherObjectVoidPtr = nullptr;
	if (Value.count("OtherObject") > 0)
	{
		OtherObjectVoidPtr = Value.at("OtherObject");
	}
	if (OtherObjectVoidPtr)
	{
		Object* OtherObject = static_cast<Object*>(OtherObjectVoidPtr);
		if (OtherObject && OtherObject->HasTag("Player"))
		{
			//change scene
			if (World().GetCurrentSceneName() == "FirstScene")
			{
				World().Load("SecondScene");
			}
			else if (World().GetCurrentSceneName() == "SecondScene")
			{
				World().Load("ThirdScene");
			}
			else if (World().GetCurrentSceneName() == "ThirdScene")
			{
				World().Load("MenuScene");
				World().SetPersistentValueF("PlayerHealth", 100.f);
			}
			else
			{
				World().Load("MenuScene");
				World().SetPersistentValueF("PlayerHealth", 100.f);
			}
		}
	}
}
