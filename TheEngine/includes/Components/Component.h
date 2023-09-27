#pragma once

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

	protected:
		Object* mOwner = nullptr;
	};
}