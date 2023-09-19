#pragma once

namespace Core
{
	class IWorld
	{
	public:
		virtual ~IWorld() = default;
		virtual void Update(float DetaTime) = 0;
		virtual void Draw() = 0;

		/**
		*	Factory method for creating Objects.
		*	Don't forget to call Init() on the returned object for complete initialization.
		*/
		template <typename T>
		virtual T* CreateObject() {};
	};
}