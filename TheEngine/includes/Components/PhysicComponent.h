#pragma once

#include "Component.h"
#include "Interfaces/IUpdatable.h"
#include "Utility.h"
#include "Interfaces/IObserver.h"

namespace Core
{
	class CollisionComponent;

	/** represents the side of a rect. */
	enum class ECollisionSide
	{
		Left = 0,
		Right = 1,
		Top = 2,
		Bot = 3,

		MAX
	};

	class PhysicComponent : public Component, public IUpdatable, public IObserverThreeParams<Object*, CollisionComponent*, const Vector<float>&>
	{
	public:
		using T1 = Object*;
		using T2 = CollisionComponent*;
		using T3 = const Vector<float>&;

		PhysicComponent(Object* Owner);
		virtual ~PhysicComponent() = default;
		virtual void Start();
		virtual void Destroy();
		virtual void Update(float DeltaTime) override;
		
		void AddMovement(const Vector<float>& MovementDirection);

	private:
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
		Vector<float> mDesiredMoveThisFrame = Vector<float>::ZeroVector();
		Vector<float> mPendingMove = Vector<float>::ZeroVector();
		Vector<float> mOldLocation = Vector<float>::ZeroVector();

		CollisionComponent* mCollisionComponent = nullptr;
		bool bCollisionOccured = false;
		Vector<float> mCollisionPoint = Vector<float>::ZeroVector();

		ECollisionSide GetCollisionNearestSide();
		float DistancePointToLine(Vector<float> Line[2]);


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


		// Hérité via IObserverThreeParams
		virtual void OnNotify(const T1& Value1, const T2& Value2, const T3& Value3) override;

	};
}