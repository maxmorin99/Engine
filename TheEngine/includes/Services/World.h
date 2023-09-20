#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace Core
{
	class Object;

	class World final
	{
	public:
		World() = default;
		virtual ~World() = default;
		void Update(float DetaTime);
		void Draw();

		template <typename T>
		T* CreateObject();

	private:
		std::unordered_map<std::string, Object*> mObjects;

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
			mObjects[Obj->GetId()] = Obj;
			Obj->Init();
		}

		return NewTObject;
	}
}