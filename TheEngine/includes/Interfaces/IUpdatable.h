#pragma once

namespace Core
{
	class IUpdatable
	{
	public:
		virtual void Update(float DeltaTime) = 0;
	};
}