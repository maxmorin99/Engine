#pragma once

#include "Interfaces/IWorld.h"
#include <vector>
#include <unordered_map>

namespace Core
{
	class World final : public IWorld
	{
	public:
		World() = default;
		virtual ~World() = default;
		virtual void Start() override;
		virtual void Update(float DeltaTime) override;
		virtual void Render() override;
		virtual void Destroy(Object* Obj) override;
		void AddObject(Object* Obj);
		virtual void Register(const std::string& SceneName, IScene* Scene) override;
		virtual void Load(const std::string& SceneName) override;
		virtual void Unload() override;

	private:
		std::unordered_map<std::string, Object*> mObjectMap;
		std::vector<Object*> mObjectList;
		std::vector<Object*> mToDestroyList;
		std::unordered_map < std::string, IScene*> mSceneMap;
		IScene* mCurrentScene = nullptr;


		std::vector<Object*>::const_iterator GetObjectIt(const Object* InObj) const;
	};
}