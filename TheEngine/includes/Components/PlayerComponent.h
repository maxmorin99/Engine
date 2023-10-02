#pragma once

#include "Component.h"
#include "Interfaces/IUpdatable.h"

namespace Core
{
	class IInput;

	class PlayerComponent : public Component, public IUpdatable
	{
	public:
		PlayerComponent(Object* Owner);
		virtual ~PlayerComponent() = default;
		virtual void Start();
		virtual void Destroy();

		virtual void Update(float DeltaTime) override;

	
	};
}