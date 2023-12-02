#pragma once

#include <string>

namespace Core
{
	class Object;

	class ISpawner
	{
	public:

		/**
		* @brief Adds a prototype object to the spawner.
		* @param name The name associated with the prototype object.
		* @param Obj A pointer to the Object prototype to be added.
		*/
		virtual void AddPrototype(const std::string& name, Object* Obj) = 0;

		/**
		* @brief Spawns a new object based on a given prototype name.
		* @param name The name of the prototype to spawn.
		*/
		virtual Object* Spawn(const std::string& name) = 0;

		/**
		* @brief Shuts down the spawner, releasing any allocated resources.
		*/
		virtual void Shutdown() = 0;
	};
}