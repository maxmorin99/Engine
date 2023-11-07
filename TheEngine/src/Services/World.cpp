#include "Services/World.h"
#include <sstream>
#include "Object.h"
#include "Interfaces/IScene.h"
#include "Components/BoxComponent.h"
#include "Services/Collision.h"

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
	CheckWorldCollision();
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
	Vector<float> CollisionPoint = Vector<float>::ZeroVector();

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
			
			bCollision = Collision::RectWithRect(Box1->GetRect(), Box2->GetRect(), CollisionPoint);
			if (bCollision)
			{
				Object* Obj1 = Comp1->GetOwner();
				Object* Obj2 = Comp2->GetOwner();

				switch (Response)
				{
				case ECollisionResponse::Block:
					Comp1->OnCollisionHit(Obj2, Comp2, CollisionPoint);
					Comp2->OnCollisionHit(Obj1, Comp1, CollisionPoint);
					break;
				case ECollisionResponse::Overlap:
					if (!Comp1->IsOverlappingWith(Obj2))
					{
						Comp1->OnCollisionOverlapBegin(Obj2, Comp2, CollisionPoint);
						Comp1->AddOverlappingObject(Obj2);
					}
					if (!Comp2->IsOverlappingWith(Obj1))
					{
						Comp2->OnCollisionOverlapBegin(Obj1, Comp1, CollisionPoint);
						Comp2->AddOverlappingObject(Obj1);
					}
					
					break;
				default:
					break;
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
			OtherCollisionComp.push_back(Comps[i]);
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
	for (auto& pair : mSceneMap)
	{
		IScene* S = pair.second;
		S->Destroy();
		delete S;
	}
	mSceneMap.clear();
}

void Core::World::AddCollisionComponent(CollisionComponent* Comp)
{
	if (!Comp) return;

	ECollisionChannel Channel = Comp->GetCollisionChannel();
	if (mCollisionComponents.count(Channel) == 0)
	{
		std::vector<CollisionComponent*> CompList;
		CompList.push_back(Comp);
		mCollisionComponents[Channel] = CompList;
	}
	else
	{
		std::vector<CollisionComponent*> CompList = mCollisionComponents.at(Channel);
		CompList.push_back(Comp);
		mCollisionComponents[Channel] = CompList;
	}
}