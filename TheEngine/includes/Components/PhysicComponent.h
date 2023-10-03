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
		/** Maximum velocity length allowed for movement */
		float mMaxMovementSpeed = 100.f;
		float mDecelerationSpeed = 50.f;
		float mAccelerationSpeed = 100.f;

		/** 
		*	The greater this value is, the more time the object takes to accelerate/deccelerate
		*	and the more slidy it will be
		*/
		float mMass = 0.f;
		float mSlideFactor = 0.f;
		Vector<float> mVelocity = Vector<float>::ZeroVector();
		Vector<float> mPendingMove = Vector<float>::ZeroVector();

	public:
		inline void SetAccelerationSpeed(float NewSpeed) { mAccelerationSpeed = NewSpeed; }
		inline float GetAccelerationSpeed() const { return mAccelerationSpeed; }
		inline void SetDecelerationSpeed(float NewSpeed) { mDecelerationSpeed = NewSpeed; }
		inline float GetDecelerationSpeed() const { return mDecelerationSpeed; }
		inline void SetMaxMovementSpeed(float NewSpeed) { mMaxMovementSpeed = NewSpeed; }
		inline float GetMaxMovementSpeed() const { return mMaxMovementSpeed; }
		void SetSlideFactor(float Factor);
		inline float GetSlideFactor() const { return mSlideFactor; }
		void SetMass(float Mass);
		inline float GetMass() const { return mMass; }
		inline Vector<float> GetVelocity() const { return mVelocity; }
	};
}