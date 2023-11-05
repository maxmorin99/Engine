#pragma once

#include "Interfaces/IWorld.h"
#include <vector>
#include <unordered_map>
#include "Utility.h"

namespace Core
{
	class ILogger;

	class World final : public IWorld
	{
	public:
		World(ILogger& Logger);
		virtual ~World() = default;
		virtual void Start() override;
		virtual void Update(float DeltaTime) override;
		virtual void Render() override;
		virtual void Destroy(Object* Obj) override;
		void AddObject(Object* Obj);
		virtual void Register(const std::string& SceneName, IScene* Scene) override;
		virtual void Load(const std::string& SceneName) override;
		virtual void Unload() override;
		virtual void ShutDown() override;

		/** Register collision component in the world for collision */
		virtual void AddCollisionComponent(CollisionComponent* Comp) override;

	private:
		/** Map of objects <object Id, Object Ref> */
		std::unordered_map<std::string, Object*> mObjectMap;

		/** Raw list of objects */
		std::vector<Object*> mObjectList;

		/** Raw list of objects pending destroy */
		std::vector<Object*> mToDestroyList;

		/** Map of scenes <Scene name, scene ref> */
		std::unordered_map < std::string, IScene*> mSceneMap;

		IScene* mCurrentScene = nullptr;

		/** Collision components of different category (ennemy, player, bullets, etc) */
		std::unordered_map<ECollisionChannel, std::vector<CollisionComponent*>> mCollisionComponents;

		ILogger& mLogger;

		/** Wheter or not a scene change was requested (false if it is the first scene loaded) */
		bool bChangeSceneRequested = false;

		/** Get object Iterator */
		std::vector<Object*>::const_iterator GetObjectIt(const Object* InObj) const;

		void UpdateObjects(float DeltaTime);
		void DeleteObjects();
		void CheckObjectsForStart();
		void CheckWorldCollision();
	};
}