#pragma once

#include "Interfaces/IScene.h"
#include <string>

namespace Core
{
	class Scene : public IScene
	{
	public:
		Scene(const char* name);
		virtual void Load() override;

	private:
		std::string mName = "\0";
	};
}