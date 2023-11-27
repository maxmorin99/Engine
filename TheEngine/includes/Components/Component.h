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
		virtual Component* Clone(Object* Owner) { return nullptr; };
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
		Object* GetOwner() const { return mOwner; }
	};
}