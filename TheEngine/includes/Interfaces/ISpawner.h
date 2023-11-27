#pragma once

#include <string>

namespace Core
{
	class Object;

	class ISpawner
	{
	public:
		virtual void AddPrototype(const std::string& name, Object* Obj) = 0;
		virtual Object* Spawn(const std::string& name) = 0;
		virtual void Shutdown() = 0;
	};
}