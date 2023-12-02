#pragma once
#include "Engine/Core.h"

namespace Core
{
	class Object;

	class Component
	{
	public:
		Component(Object* Owner);
		virtual ~Component() = default;
		virtual void Start() {};
		virtual void Destroy() {};

		/** Called to clone this component and all important member variables
		*	@param Owner: Object that will own the the clone component
		*/
		virtual Component* Clone(Object* Owner) { return nullptr; };

		/** If a child component calls Clone(), it will call SetupClone to clone all important member variables of its parents component
		*	@param Child: Child component of this class that needs each of its parent clone version.
		*/
		virtual void SetupClone(Component* Child) {};

	protected:
		Object* mOwner = nullptr;

		IInput& Input() const;
		IGraphic& Graphic() const;
		ITimer& Timer() const;
		IAudio& Audio() const;
		ILogger& Logger() const;
		IWorld& World() const;
		ISpawner& Spawner() const;

	public:
		/** Get the object that owns this component */
		Object* GetOwner() const { return mOwner; }
	};
}