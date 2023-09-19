#pragma once

namespace Core
{
	class IWorld
	{
	public:
		virtual ~IWorld() = default;
		virtual void Update(float DetaTime) = 0;
		virtual void Draw() = 0;
	};
}