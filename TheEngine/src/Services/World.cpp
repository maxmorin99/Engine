#include "Services/World.h"
#include <sstream>
#include "Object.h"
#include "Interfaces/IScene.h"

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

void Core::World::Start()
{
	for (int i = 0; i < mObjectList.size(); i++)
	{
		Object* Obj = mObjectList[i];
		if (Obj)
		{
			Obj->Start();
		}
	}
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

void Core::World::Render()
{
	for (int i = 0; i < mObjectList.size(); i++)
	{
		Object* Obj = mObjectList[i];
		if (Obj)
		{
			Obj->Render();
		}
	}
}

void Core::World::Destroy(Object* Obj)
{
	if (!Obj) return;
	int count = 0;
	for (Object* Obj : mToDestroyList)
	{
		if (Obj == Obj)
		{
			count++;
		}
	}
	if (count > 0) return;
	mToDestroyList.push_back(Obj);
}

void Core::World::Register(const std::string& SceneName, IScene* Scene)
{
	if (mSceneMap.count(SceneName) > 0 || !Scene) return;
	mSceneMap[SceneName] = Scene;
}

void Core::World::Load(const std::string& SceneName)
{
	if (mSceneMap.count(SceneName) == 0) return;
	IScene* Scene = mSceneMap[SceneName];
	if (Scene)
	{
		Unload();
		mCurrentScene = Scene;
		mCurrentScene->Load();
	}
}

void Core::World::Unload()
{
	if (!mCurrentScene) return;
	for (Object* Obj : mObjectList)
	{
		Destroy(Obj);
	}
	mObjectList.clear();
	mObjectMap.clear();
	mToDestroyList.clear();
	mCurrentScene = nullptr;
}

void Core::World::AddObject(Object* Obj)
{
	if (!Obj ||  mObjectMap.count(Obj->GetId()) > 0) return;
	mObjectList.push_back(Obj);
	mObjectMap[Obj->GetId()] = Obj;
}

void Core::World::ShutDown()
{
	for (auto& S : mSceneMap)
	{
		delete S.second;
	}
}