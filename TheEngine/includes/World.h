#pragma once

#include "Interfaces/IWorld.h"
#include <vector>
#include <unordered_map>

namespace Core
{
	class Object;

	class World final : public IWorld
	{
	public:
		// Hérité via IWorld
		virtual ~World();
		World();
		virtual void Update(float DetaTime) override;
		virtual void Draw() override;

		template <typename T>
		T* CreateObject();

	private:
		std::unordered_map<size_t, Object*> mObjects;

		void SetObjectId(Object* InObj) const;

	};

	template<typename T>
	inline T* World::CreateObject()
	{
		T* NewTObject = new T();
		Object* Obj = dynamic_cast<Object*>(NewTObject);
		if (Obj)
		{
			SetObjectId(Obj);
		}

		return NewTObject;
	}
}