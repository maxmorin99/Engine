#include "Components/PathFindingComponent.h"
#include "Components/TilemapComponent.h"
#include "Object.h"

Core::PathFindingComponent::PathFindingComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PathFindingComponent::Start()
{
	Component::Start();

	Object* TmObj = World().GetTilemapObject();
	if (TmObj)
	{
		TilemapComponent* TmComp = TmObj->GetComponent<TilemapComponent>();
		if (TmComp)
		{
			TTilemap Tm = TmComp->GetTilemap();
			BuildMap(Tm);
		}
	}
}

void Core::PathFindingComponent::Destroy()
{
	Component::Destroy();
}

void Core::PathFindingComponent::BuildMap(TTilemap Tilemap)
{

}
