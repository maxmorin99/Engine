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
		
		void AddMovement(const Vector<float>& MovementDirection);

	private:
		float mMaxMovementSpeed = 100.f;
		float mMovementSpeed = 100.f;
		float mMass = 0.f;
		Vector<float> mVelocity = Vector<float>::ZeroVector();

	public:
		inline void SetMovementSpeed(float NewSpeed) { mMovementSpeed = NewSpeed; }
		inline float GetMovementSpeed() const { return mMovementSpeed; }
		inline void SetMaxMovementSpeed(float NewSpeed) { mMaxMovementSpeed = NewSpeed; }
		inline float GetMaxMovementSpeed() const { return mMaxMovementSpeed; }
		inline void SetMass(float Mass) { mMass = Mass; }
		inline float GetMass() const { return mMass; }
		inline Vector<float> GetVelocity() const { return mVelocity; }
	};
}