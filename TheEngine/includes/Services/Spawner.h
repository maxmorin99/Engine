#pragma once

#include <unordered_map>

#include "Interfaces/ISpawner.h"

namespace Core
{
	class Spawner : public ISpawner
	{
	public:
		Spawner();
		virtual ~Spawner() = default;
		virtual void AddPrototype(const std::string& name, Object* Obj) override;
		virtual Object* Spawn(const std::string& name) override;
		virtual void Shutdown() override;

	private:
		std::unordered_map<std::string, Object*>* mProtoMap;
	};
}