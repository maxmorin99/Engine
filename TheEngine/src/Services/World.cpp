#include "Services/World.h"
#include <sstream>
#include "Object.h"
#include "Interfaces/IScene.h"
#include "Components/BoxComponent.h"
#include "Services/Collision.h"
#include "Components/TilemapComponent.h"

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
	// CurrentScene will have objects to add
	CheckObjectsToAdd();

	// Call Start on all registered objects
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
	// Check if there is Objects to add to the world this frame
	CheckObjectsToAdd();

	// Update all objects
	UpdateObjects(DeltaTime);

	// Check and process collisions
	CheckWorldCollision();

	// If there is pending kill objects, delete them
	DeleteObjects();
	
	// If we changed scene, call start on objects newly added
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
		DeleteObjectFromObjectMap(Obj);

		// erase from object list
		DeleteObjectFromObjectList(Obj);

		Obj->Destroy();
		delete Obj;
	}
	mToDestroyList.clear();

	// remove collision components from map
	RemoveCollisionComponentsFromMap();
}

void Core::World::DeleteObjectFromObjectMap(Object* Obj)
{
	// earse from map
	std::string ObjId = Obj->GetId();
	if (mObjectMap.count(ObjId) > 0)
	{
		mObjectMap.erase(ObjId);
	}
}

void Core::World::DeleteObjectFromObjectList(Object* Obj)
{
	std::vector<Object*>::const_iterator It = GetObjectIt(Obj);
	if (mObjectList.size() > 0 && It != mObjectList.end())
	{
		mObjectList.erase(It);
	}
}

void Core::World::CheckObjectsForStart()
{
	if (bChangeSceneRequested)
	{
		// Call start on all object if we changed scene
		for (Object* Obj : mObjectList)
		{
			Obj->Start();
		}
		bChangeSceneRequested = false;
	}

	for (Object* Obj : mToAddList)
	{
		Obj->Start();
	}
}

void Core::World::CheckObjectsToAdd()
{
	// Add Objects
	AddPendingObjects();

	// Add Collision Components
	AddPendingCollisionComponent();
}

void Core::World::AddPendingObjects()
{
	for (Object* Obj : mToAddList)
	{
		if (mObjectMap.count(Obj->GetId()) > 0) return;
		mObjectList.push_back(Obj);
		mObjectMap[Obj->GetId()] = Obj;
	}
	mToAddList.clear();
}

void Core::World::AddPendingCollisionComponent()
{
	for (auto& pair : mCollisionComponentsToAdd)
	{
		ECollisionChannel Channel = pair.first;

		if (mCollisionComponents.count(Channel) == 0)
		{
			std::vector<CollisionComponent*> CompList;
			for (int i = 0; i < pair.second.size(); i++)
			{
				CompList.push_back(pair.second[i]);
			}
			mCollisionComponentsToAdd[Channel].clear();
			mCollisionComponents[Channel] = CompList;
		}
		else
		{
			std::vector<CollisionComponent*> CompList = mCollisionComponents.at(Channel);
			for (int i = 0; i < pair.second.size(); i++)
			{
				CompList.push_back(pair.second[i]);
			}
			mCollisionComponentsToAdd[Channel].clear();
			mCollisionComponents[Channel] = CompList;
		}
	}
	mCollisionComponentsToAdd.clear();
}

void Core::World::RemoveCollisionComponentsFromMap()
{
	for (auto& pair : mCollisionComponentsToDel)
	{
		ECollisionChannel Channel = pair.first;
		std::vector<CollisionComponent*> CompListToDel = mCollisionComponentsToDel[Channel];

		for (int i = 0; i < mCollisionComponentsToDel[Channel].size(); i++)
		{
			for (int j = 0; j < mCollisionComponents[Channel].size(); j++)
			{
				if (mCollisionComponentsToDel[Channel][i] == mCollisionComponents[Channel][j])
				{
					mCollisionComponents[Channel].erase(mCollisionComponents[Channel].begin() + j);
				}
			}
		}
		mCollisionComponentsToDel[Channel].clear();
	}
	mCollisionComponentsToDel.clear();
}

void Core::World::CheckWorldCollision()
{
	for (auto& pair : mCollisionComponents)
	{
		std::vector<CollisionComponent*> ColComps = pair.second;

		for (int i = 0; i < ColComps.size(); i++)
		{
			std::vector<ECollisionChannel> Channels = GetListOfChannelsThatCanCollide(ColComps[i]);

			// this is a list of all the CollisionComponents* that ColComps[i] can collide with
 			std::vector<CollisionComponent*> OtherCollisionComps = GetAllCollisionComponentOfChannels(Channels);

			for (int j = 0; j < OtherCollisionComps.size(); j++)
			{
				// not process the same collision
				if (ColComps[i] == OtherCollisionComps[j]) continue;

				ECollisionResponse LeastSevereResponse = GetLeastSevereCollisionResponse(ColComps[i], OtherCollisionComps[j]);
				if (LeastSevereResponse != ECollisionResponse::Ignore)
				{
					ProcessCollision(LeastSevereResponse, ColComps[i], OtherCollisionComps[j]);
				}
			}
		}
	}
}

Core::ECollisionResponse Core::World::GetLeastSevereCollisionResponse(CollisionComponent* Comp1, CollisionComponent* Comp2)
{
	// Return the least severe collision between the two compared components
	if (!Comp1 || !Comp2) return ECollisionResponse::Ignore;

	ECollisionChannel ChannelComp1 = Comp1->GetCollisionChannel();
	ECollisionChannel ChannelComp2 = Comp2->GetCollisionChannel();

	ECollisionResponse Comp1ResponseToComp2Channel = Comp1->GetCollisionResponseToChannel(ChannelComp2);
	ECollisionResponse Comp2ResponseToComp1Channel = Comp2->GetCollisionResponseToChannel(ChannelComp1);

	if (Comp1ResponseToComp2Channel == Comp2ResponseToComp1Channel)
	{
		return Comp1ResponseToComp2Channel;
	}
	if (Comp1ResponseToComp2Channel == ECollisionResponse::Ignore || Comp2ResponseToComp1Channel == ECollisionResponse::Ignore)
	{
		return ECollisionResponse::Ignore;
	}
	if (Comp1ResponseToComp2Channel == ECollisionResponse::Overlap || Comp2ResponseToComp1Channel == ECollisionResponse::Overlap)
	{
		return ECollisionResponse::Overlap;
	}
	return ECollisionResponse::Block;
}

void Core::World::ProcessCollision(const ECollisionResponse& Response, CollisionComponent* Comp1, CollisionComponent* Comp2)
{
	if (!Comp1 || !Comp2) return;

	ECollisionShape Shape1 = Comp1->GetCollisionType();
	ECollisionShape Shape2 = Comp2->GetCollisionType();
	ECollisionSide Side{ ECollisionSide::Undefined };

	if (Shape1 == Shape2)
	{
		BoxComponent* Box1 = nullptr;
		BoxComponent* Box2 = nullptr;
		bool bCollision = false;
		switch (Shape1)
		{
		case ECollisionShape::Rectangle:
			// check collision with 2 rects
			Box1 = dynamic_cast<BoxComponent*>(Comp1);
			Box2 = dynamic_cast<BoxComponent*>(Comp2);
			if (!Box1 || !Box2) return;
			
			bCollision = Collision::RectWithRect(Box1->GetRect(), Box2->GetRect(), Side);
			if (bCollision)
			{
				Object* Obj1 = Comp1->GetOwner();
				Object* Obj2 = Comp2->GetOwner();

				switch (Response)
				{
				case ECollisionResponse::Block:
					Comp1->OnCollisionHit(Obj2, Comp2);
					Comp2->OnCollisionHit(Obj1, Comp1);
					
					break;
				case ECollisionResponse::Overlap:
					if (!Comp1->IsOverlappingWith(Obj2))
					{
						Comp1->OnCollisionOverlapBegin(Obj2, Comp2);
						Comp1->AddOverlappingObject(Obj2);
					}
					if (!Comp2->IsOverlappingWith(Obj1))
					{
						Comp2->OnCollisionOverlapBegin(Obj1, Comp1);
						Comp2->AddOverlappingObject(Obj1);
					}
					break;
				default:
					break;
				}
			}
			else
			{
				Object* Obj1 = Comp1->GetOwner();
				Object* Obj2 = Comp2->GetOwner();

				if (Comp1->IsOverlappingWith(Obj2))
				{
					Comp1->RemoveOverlappingObject(Obj2);
				}
				if (Comp2->IsOverlappingWith(Obj1))
				{
					Comp2->RemoveOverlappingObject(Obj1);
				}
			}
			break;
		case ECollisionShape::Circle:
			// check collision with 2 circles
			break;
		case ECollisionShape::Line:
			//check collision with 2 lines
			break;
		}
	}
	else if (Shape1 == ECollisionShape::Rectangle)
	{
		if (Shape2 == ECollisionShape::Circle)
		{
			// check collision Rect / Circle
		}
		else if (Shape2 == ECollisionShape::Line)
		{
			// check collision Rect / Line
		}
	}
	else if (Shape1 == ECollisionShape::Circle)
	{
		if (Shape2 == ECollisionShape::Rectangle)
		{
			// check collision Circle / Rect
		}
		else if (Shape2 == ECollisionShape::Line)
		{
			// check collision Circle / Line
		}
	}
	else if (Shape1 == ECollisionShape::Line)
	{
		if (Shape2 == ECollisionShape::Rectangle)
		{
			// check collision Line / Rect
		}
		else if (Shape2 == ECollisionShape::Circle)
		{
			// check collision Line / Circle
		}
	}
}

std::vector<Core::ECollisionChannel> Core::World::GetListOfChannelsThatCanCollide(CollisionComponent* Comp) const
{
	if(!Comp) return std::vector<ECollisionChannel>();
	
	std::vector<ECollisionChannel> Channels;

	std::unordered_map CollisionResponseToChannels = Comp->GetCollisionResponseToChannels();
	for (auto& pair : CollisionResponseToChannels)
	{
		ECollisionResponse Response = pair.second;
		if (Response != ECollisionResponse::Ignore)
		{
			Channels.push_back(pair.first);
		}
	}

	return Channels;
}

std::vector<Core::CollisionComponent*> Core::World::GetAllCollisionComponentOfChannels(const std::vector<ECollisionChannel>& ChannelList)
{
	std::vector<CollisionComponent*> OtherCollisionComp;
	for (int i = 0; i < ChannelList.size(); i++)
	{
		std::vector<CollisionComponent*> Comps = mCollisionComponents[ChannelList[i]];
		for (int k = 0; k < Comps.size(); k++)
		{
			OtherCollisionComp.push_back(Comps[k]);
		}
	}
	return OtherCollisionComp;
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

		// Add object's collision comp to destroy list too
		CollisionComponent* ColComp = Obj->GetCollisionComponent();
		if (ColComp)
		{
			ECollisionChannel Channel = ColComp->GetCollisionChannel();
			mCollisionComponentsToDel[Channel].push_back(ColComp);
		}
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
	for (Object* Obj : mToAddList)
	{
		Destroy(Obj);
	}

	mObjectList.clear();
	mObjectMap.clear();
	mToAddList.clear();
	mCollisionComponentsToAdd.clear();
	mCurrentScene = nullptr;
}

void Core::World::AddObject(Object* Obj)
{
	bool bContains = false;
	for (Object* O : mToAddList)
	{
		if (Obj == O)
		{
			bContains = true;
			break;
		}
	}

	if (!Obj || bContains) return;
	mToAddList.push_back(Obj);

	CollisionComponent* ColComp = Obj->GetCollisionComponent();
	if (ColComp)
	{
		AddCollisionComponent(ColComp);
	}
}

void Core::World::ShutDown()
{
	for (auto& pair : mSceneMap)
	{
		IScene* S = pair.second;
		S->Destroy();
		delete S;
	}
	mSceneMap.clear();
}

Core::Object* Core::World::GetTilemapObject() const
{
	for (Object* Obj : mObjectList)
	{
		TilemapComponent* Tm = Obj->GetComponent<TilemapComponent>();
		if (Tm)
		{
			return Obj;
		}
	}
	return nullptr;
}

void Core::World::AddCollisionComponent(CollisionComponent* Comp)
{
	if (!Comp) return;

	ECollisionChannel Channel = Comp->GetCollisionChannel();

	if (mCollisionComponentsToAdd.count(Channel) == 0)
	{
		std::vector<CollisionComponent*> CompList;
		CompList.push_back(Comp);
		mCollisionComponentsToAdd[Channel] = CompList;
	}
	else
	{
		std::vector<CollisionComponent*> CompList = mCollisionComponentsToAdd.at(Channel);
		CompList.push_back(Comp);
		mCollisionComponentsToAdd[Channel] = CompList;
	}
}