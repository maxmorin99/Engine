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
		virtual Object* GetTilemapObject() const override;
		virtual std::vector<Object*> GetObjectsWithTag(const std::string& Tag) const override;
		virtual std::string GetCurrentSceneName() const override;
		virtual void SetPersistentValueF(const std::string& ValueName, float Value) override;
		virtual float GetPersistentValueF(const std::string& ValueName) const override;

	private:
		/** Map of objects <object Id, Object Ref> */
		std::unordered_map<std::string, Object*> mObjectMap;

		/** Raw list of objects */
		std::vector<Object*> mObjectList;

		/** Raw list of objects pending destroy */
		std::vector<Object*> mToDestroyList;

		/** Raw list of objects pending add */
		std::vector<Object*> mToAddList;

		/** Map of scenes <Scene name, scene ref> */
		std::unordered_map <std::string, IScene*> mSceneMap;

		/** Collision components of different category (ennemy, player, bullets, etc) */
		std::unordered_map<ECollisionChannel, std::vector<CollisionComponent*>> mCollisionComponents;

		/** Map of CollisionComponent* to add to mCollisionComponents */
		std::unordered_map<ECollisionChannel, std::vector<CollisionComponent*>> mCollisionComponentsToAdd;

		/** Map of CollisionComponent* to remove from mCollisionComponents */
		std::unordered_map<ECollisionChannel, std::vector<CollisionComponent*>> mCollisionComponentsToDel;

		IScene* mCurrentScene = nullptr;
		ILogger& mLogger;

		/** Wheter or not a scene change was requested (false if it is the first scene loaded) */
		bool bChangeSceneRequested = false;

		/** Map of all the persistent values */
		std::unordered_map<std::string, float> mPersistentValuesMapF;


		/** Get object Iterator */
		std::vector<Object*>::const_iterator GetObjectIt(const Object* InObj) const;

		/** Update Objects */
		void UpdateObjects(float DeltaTime);

		/** Delete Objects that are il Delete list */
		void DeleteObjects();

		/** Remove Object from mObjectMap */
		void DeleteObjectFromObjectMap(Object* Obj);

		/** Remove Object from mObjectList */
		void DeleteObjectFromObjectList(Object* Obj);

		/** If there are new Objects (dynamically instanciated objects or by changing scene) call Start on these Objects */
		void CheckObjectsForStart();

		/** Verify if there is Objects to add thi frame */
		void CheckObjectsToAdd();

		/** Add Objects to the mObjectList and mObjectMap */
		void AddPendingObjects();

		/** Add Collision Components to the mCollisionComponents map */
		void AddPendingCollisionComponent();

		void RemoveCollisionComponentsFromMap();


		/** Collisions ------------------------------------------------------------------------------------------------------------------  */

		/** Register collision component in the world for collision */
		virtual void AddCollisionComponent(CollisionComponent* Comp) override;

		/** Get the least severe CollisionResponse between 2 CollisionComponents
		*	For exemple, if Comp1 has a response Block and Comp2 has a response Ignore, Ignore will be returned.
		*	Refer to this order: Ignore, Overlap, Block
		*/
		ECollisionResponse GetLeastSevereCollisionResponse(CollisionComponent* Comp1, CollisionComponent* Comp2);

		void CheckWorldCollision();

		/** Determine if both components are colliding */
		void ProcessCollision(const ECollisionResponse& Response, CollisionComponent* Comp1, CollisionComponent* Comp2);

		/** Get a list of CollisionChannels that a certain component can collide with.
		*	A component can collide with a channel if the CollisionResponse is not set to Ignore.
		*/
		std::vector<ECollisionChannel> GetListOfChannelsThatCanCollide(CollisionComponent* Comp) const;

		/** Get a list of all the CollisionComponent* in mCollisionComponents under a channel from ChannelList */
		std::vector<CollisionComponent*> GetAllCollisionComponentOfChannels(const std::vector<ECollisionChannel>& ChannelList);
	};
}