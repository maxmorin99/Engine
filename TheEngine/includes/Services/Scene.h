#pragma once

#include "Interfaces/IScene.h"
#include <string>

namespace Core
{
	class Scene final : public IScene
	{
	public:
		Scene(const char* name);
		void Load() override;

	private:
		std::string mName = "\0";
	};
}