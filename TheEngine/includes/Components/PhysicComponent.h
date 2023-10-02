#pragma once

#include "Component.h"
#include "Interfaces/IUpdatable.h"
#include "Utility.h"

namespace Core
{
	class PhysicComponent : public Component, public IUpdatable
	{
	public:
		PhysicComponent(Object* Owner);
		virtual ~PhysicComponent() = default;
		virtual void Start();
		virtual void Destroy();

		virtual void Update(float DeltaTime) override;
		
		void AddMovement(const Vector<int>& MovementDirection);

	private:
		float mMovementSpeed = 100.f;

	public:
		inline void SetMovementSpeed(float NewSpeed) { mMovementSpeed = NewSpeed; }
		inline float GetMovementSpeed() const { return mMovementSpeed; }
	};

}