#pragma once

#include <string>

namespace Core
{
	class IScene
	{
	public:
		virtual ~IScene() = default;
		virtual void Load() = 0;
		virtual void Destroy() = 0;
		virtual void UnLoad() = 0;
		virtual std::string GetName() = 0;
	};
}