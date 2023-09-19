#pragma once
#include <unordered_map>
#include "Object.h"

namespace Core
{
	class ObjectFactory final
	{
	public:
		/**
		*	Factory method for creating Objects.
		*	Don't forget to call Init() on the returned object for complete initialization.
		*/
		template <typename T>
		T* CreateObject();
	};

	template<typename T>
	inline T* ObjectFactory::CreateObject()
	{
		T* NewTObject = new T();
		Object* NewObject = dynamic_cast<Object*>(NewTObject);
		if (NewObject)
		{
			size_t Id = std::hash<size_t>()(NewObject);
			NewObject->SetId(Id);
			NewObject->Init();
		}
		return NewObject;
	}
}