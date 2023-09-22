#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace Core
{
	class Object;

	class World
	{
	public:
		World() = default;
		virtual ~World() = default;
		void Update(float DeltaTime);
		void Draw();
		void Destroy(Object* Obj);

		template <typename T>
		T* CreateObject();

	private:
		std::unordered_map<std::string, Object*> mObjectMap;
		std::vector<Object*> mObjectList;
		std::vector<Object*> mToDestroyList;


		void SetObjectId(Object* InObj) const;
		std::vector<Object*>::const_iterator GetObjectIt(const Object* InObj) const;
	};

	template<typename T>
	inline T* World::CreateObject()
	{
		T* NewTObject = new T();
		Object* Obj = dynamic_cast<Object*>(NewTObject);
		if (Obj)
		{
			SetObjectId(Obj);
			mObjectMap[Obj->GetId()] = Obj;
			mObjectList.push_back(Obj);
			Obj->Start();
		}

		return NewTObject;
	}
}