#include "Services/World.h"
#include <sstream>
#include "Object.h"

void Core::World::SetObjectId(Object* InObj) const
{
	std::ostringstream Oss;
	Oss << InObj;
	std::string IdStr = Oss.str();
	InObj->SetId(IdStr);
}

std::vector<Core::Object*>::const_iterator Core::World::GetObjectIt(const Object* InObj) const
{
	std::vector<Object*>::const_iterator It;
	for (It = std::begin(mObjectList); It != mObjectList.end(); It++)
	{
		if (*It == InObj)
		{
			return It;
		}
	}
	
	return It;
}

void Core::World::Update(float DeltaTime)
{
	for (int i = 0; i < mObjectList.size(); i++)
	{
		Object* Obj = mObjectList[i];
		if (Obj)
		{
			Obj->Update(DeltaTime);
		}
	}
	
	// Delete objects in delete list
	for (Object* Obj : mToDestroyList)
	{
		// earse from map
		std::string ObjId = Obj->GetId();
		mObjectMap.erase(ObjId);

		// erase from object list
		std::vector<Object*>::const_iterator It = GetObjectIt(Obj);
		mObjectList.erase(It);

		delete Obj;
	}
	mToDestroyList.clear();
}

void Core::World::Draw()
{
	for (int i = 0; i < mObjectList.size(); i++)
	{
		Object* Obj = mObjectList[i];
		if (Obj)
		{
			Obj->Draw();
		}
	}
}

void Core::World::Destroy(Object* Obj)
{
	if (!Obj) return;
	mToDestroyList.push_back(Obj);
}