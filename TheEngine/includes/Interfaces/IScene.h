#pragma once

#include <string>

namespace Core
{
	class IScene
	{
	public:
		virtual ~IScene() = default;

		/**
		* @brief Loads the resources and initializes the scene.
		*/
		virtual void Load() = 0;

		/**
		* @brief Destroys the scene, releasing any allocated resources.
		*/
		virtual void Destroy() = 0;

		/**
		* @brief Unloads the scene, performing any necessary cleanup.
		*/
		virtual void UnLoad() = 0;

		/**
		* @brief Retrieves the name of the scene.
		*/
		virtual std::string GetName() = 0;
	};
}