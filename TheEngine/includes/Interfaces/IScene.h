#pragma once

namespace Core
{
	class IScene
	{
	public:
		virtual ~IScene() = default;
		virtual void Load() = 0;
		virtual void Destroy() = 0;
	};
}