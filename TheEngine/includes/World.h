#pragma once

#include "Interfaces/IWorld.h"
#include <vector>
#include <unordered_map>
#include "Object.h"

namespace Core
{
	class World final : public IWorld
	{
	public:
		// Hérité via IWorld
		virtual ~World() = default;
		virtual void Update(float DetaTime) override;
		virtual void Draw() override;

		/** 
		*	Factory method for creating Objects.
		*	Don't forget to call Init() on the returned object for complete initialization.
		*/
		template <typename T>
		T* CreateObject();

	private:
		std::unordered_map<size_t, Object*> mObjects;
	};
	
	template<typename T>
	inline T* World::CreateObject()
	{
		T* NewTObject = new T();
		Object* NewObject = static_cast<Object*>(NewTObject);
		size_t Id = std::hash<size_t>()(NewObject);
		NewObject->SetId(Id);

		return nullptr;
	}

}