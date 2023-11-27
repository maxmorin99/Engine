#pragma once

#include "Component.h"
#include "Interfaces/IUpdatable.h"
#include "Utility.h"
#include "Interfaces/IObserver.h"
#include <unordered_map>

namespace Core
{
	class CollisionComponent;

	class PhysicComponent : public Component, public IUpdatable, public IObserver<std::unordered_map<std::string, void*>>
	{
	public:
		PhysicComponent(Object* Owner);
		virtual ~PhysicComponent() = default;
		virtual void Start();
		virtual void Destroy();
		virtual void Update(float DeltaTime) override;
		virtual Component* Clone(Object* Owner);
		virtual void SetupClone(Component* Child);
		
		void AddMovement(const Vector<float>& MovementDirection);

	private:
		float mMaxMovementSpeed = 100.f;
		float mDecelerationSpeed = 50.f;
		float mAccelerationSpeed = 100.f;
		bool bEnable = true;

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
		
		ECollisionSide mCollisionSides[2]{ ECollisionSide::Undefined };
		Rect<float> mCollisionRects[2]{ Rect<float>(0, 0, 0, 0) };

		void UpdateVelocity(float DeltaTime);
		void ApplyDeceleration(float DeltaTime);
		void ApplyPendingMoveToCollisionComponent();

		ECollisionSide GetCollisionSideFromOtherComponent(CollisionComponent* CollisionComp);

		bool ShouldProcessTopBotCollisionFirst(float Threshold, const Rect<float>& OwnerRect, const Rect<float>& OtherRect) const;
		void ReAdjustOwnerCollisionLocation(const ECollisionSide& CollisionSide);
		bool HandleTwoAxesInput(const Vector<float>& Dir, const Rect<float>& OwnerRect, const Rect<float>& OtherRect, ECollisionSide& OutCollisionSide);
		bool HandleOneAxisInput(const Vector<float>& Dir, const Rect<float>& OtherRect, ECollisionSide& OutCollisionSide);

		/** Build a move vector based on detected collision. This move is applied to the player and prevents him to get through colliding objects */
		void MovementCorrection(Vector<float>& CorrectedMove);

		/** Verify collision from top/bot and allow vertical movement if the collision is inside a wall. Prevent the character to get stuck in a wall */
		void VerifyVerticalCollision(Vector<float>& CorrectedMove);


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
		inline void Enable() { bEnable = true; }
		inline void Disable() { bEnable = false; }

		// Hérité via IObserver
		void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

	};
}