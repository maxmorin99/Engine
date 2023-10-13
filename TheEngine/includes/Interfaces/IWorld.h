#pragma once

#include <string>

namespace Core
{
	class Object;
	class IScene;

	class IWorld
	{
	public:
		virtual ~IWorld() = default;
		virtual void Start() = 0;
		virtual void Update(float DeltaTime) = 0;
		virtual void Render() = 0;;
		virtual void Destroy(Object* Obj) = 0;
		virtual void AddObject(Object* Obj) = 0;
		virtual void Register(const std::string& SceneName, IScene* Scene) = 0;
		virtual void Unload() = 0;
		virtual void Load(const std::string& SceneName) = 0;
		virtual void ShutDown() = 0;
	};
}