#pragma once

#include <string>
#include <vector>

namespace Core
{
	class Object;
	class IScene;
	class CollisionComponent;

	class IWorld
	{
	public:
		IWorld() = default;
		virtual ~IWorld() = default;

		/**
		* @brief Starts the world.
		*/
		virtual void Start() = 0;

		/**
		* @brief Updates the world based on the delta time.
		* @param DeltaTime The time elapsed since the last update.
		*/
		virtual void Update(float DeltaTime) = 0;

		/**
		* @brief Renders the current state of the world.
		*/
		virtual void Render() = 0;

		/**
		* @brief Destroys the specified object in the world.
		* @param Obj A pointer to the Object to be destroyed.
		*/
		virtual void Destroy(Object* Obj) = 0;

		/**
		* @brief Adds an object to the world.
		* @param Obj A pointer to the Object to be added.
		*/
		virtual void AddObject(Object* Obj) = 0;

		/**
		* @brief Adds a collision component to the world.
		* @param Comp A pointer to the CollisionComponent to be added.
		*/
		virtual void AddCollisionComponent(CollisionComponent* Comp) = 0;

		/**
		* @brief Registers a scene with the specified name in the world.
		* @param SceneName The name of the scene.
		* @param Scene A pointer to the IScene object representing the scene.
		*/
		virtual void Register(const std::string& SceneName, IScene* Scene) = 0;

		/**
		* @brief Unloads the current scene in the world.
		*/
		virtual void Unload() = 0;

		/**
		* @brief Loads the scene with the specified name in the world.
		* @param SceneName The name of the scene to be loaded.
		*/
		virtual void Load(const std::string& SceneName) = 0;

		/**
		* @brief Gets the Object representing the tilemap in the world.
		*/
		virtual Object* GetTilemapObject() const = 0;

		/**
		* @brief Gets all objects in the world with the specified tag.
		* @param Tag The tag to filter objects.
		*/
		virtual std::vector<Object*> GetObjectsWithTag(const std::string& Tag) const = 0;

		/**
		* @brief Gets the name of the currently active scene in the world.
		*/
		virtual std::string GetCurrentSceneName() const = 0;

		/**
		* @brief Sets a persistent float value in the world.
		* @param ValueName The name of the persistent value.
		* @param Value The float value to be set.
		*/
		virtual void SetPersistentValueF(const std::string& ValueName, float Value) = 0;

		/**
		* @brief Gets a persistent float value from the world.
		* @param ValueName The name of the persistent value.
		* @return The float value of the specified persistent value.
		*/
		virtual float GetPersistentValueF(const std::string& ValueName) const = 0;

		/**
		* @brief Shuts down the world, releasing any allocated resources.
		*/
		virtual void ShutDown() = 0;
	};
}