#include "Services/World.h"
#include <sstream>
#include "Object.h"
#include "Interfaces/IScene.h"

Core::World::World(ILogger& Logger) :
	mLogger(Logger)
{
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
	UpdateObjects(DeltaTime);
	DeleteObjects();
	CheckObjectsForStart();
}

void Core::World::UpdateObjects(float DeltaTime)
{
	// Don't update if we are changing scene
	if (bChangeSceneRequested) return;

	for (int i = 0; i < mObjectList.size(); i++)
	{
		Object* Obj = mObjectList[i];
		if (Obj)
		{
			Obj->Update(DeltaTime);
		}
	}
}

void Core::World::DeleteObjects()
{
	for (Object* Obj : mToDestroyList)
	{
		// earse from map
		std::string ObjId = Obj->GetId();
		if (mObjectMap.count(ObjId) > 0)
		{
			mObjectMap.erase(ObjId);
		}

		// erase from object list
		std::vector<Object*>::const_iterator It = GetObjectIt(Obj);
		if (mObjectList.size() > 0 && It != mObjectList.end())
		{
			mObjectList.erase(It);
		}

		Obj->Destroy();
		delete Obj;
	}
	mToDestroyList.clear();
}

void Core::World::CheckObjectsForStart()
{
	if (bChangeSceneRequested)
	{
		for (Object* Obj : mObjectList)
		{
			Obj->Start();
		}
		bChangeSceneRequested = false;
	}
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

	// Be sure to add only one time a ptr instance
	bool bContains = false;
	for (Object* ObjToDestroy : mToDestroyList)
	{
		if (ObjToDestroy == Obj)
		{
			bContains = true;
			break;
		}
	}
	if (!bContains)
	{
		mToDestroyList.push_back(Obj);
	}
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
		if (mCurrentScene)
		{
			Unload();
			bChangeSceneRequested = true;
		}
		
		mCurrentScene = Scene;
		mCurrentScene->Load();
	}
}

void Core::World::Unload()
{
	for (Object* Obj : mObjectList)
	{
		Destroy(Obj);
	}

	mObjectList.clear();
	mObjectMap.clear();
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
	mSceneMap.clear();
}