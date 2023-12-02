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
		
		/** Add movement to the owner object
		*	@param MovementDirection: Direction to apply the movement
		*/
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

		/** Change the velocity based on the desired move */
		void UpdateVelocity(float DeltaTime);

		/** Apply deceleration if there is no desired move */
		void ApplyDeceleration(float DeltaTime);

		/** Move the collision component to check the collisions before moving the owner */
		void ApplyPendingMoveToCollisionComponent();

		/** Get the side of the component that collided with another component
		*	@param CollisionComp: Other component that collides with the owner's collision component
		*/
		ECollisionSide GetCollisionSideFromOtherComponent(CollisionComponent* CollisionComp);

		/** Wheter or not we should process the collision checks vertically before horizontally */
		bool ShouldProcessTopBotCollisionFirst(float Threshold, const Rect<float>& OwnerRect, const Rect<float>& OtherRect) const;

		/** If there is a collision, replace the collision's location to an appropriate location */
		void ReAdjustOwnerCollisionLocation(const ECollisionSide& CollisionSide);

		/** Get the side of the collision component that collided when the player is moving in two directions at the same time */
		bool HandleTwoAxesInput(const Vector<float>& Dir, const Rect<float>& OwnerRect, const Rect<float>& OtherRect, ECollisionSide& OutCollisionSide);

		/** Get the side of the collision component that collided when the player is moving in one direction */
		bool HandleOneAxisInput(const Vector<float>& Dir, const Rect<float>& OtherRect, ECollisionSide& OutCollisionSide);

		/** Build a move vector based on detected collision. This move is applied to the player and prevents him to get through colliding objects */
		void MovementCorrection(Vector<float>& CorrectedMove);

		/** Verify collision from top/bot and allow vertical movement if the collision is inside a wall. Prevent the character to get stuck in a wall */
		void VerifyVerticalCollision(Vector<float>& CorrectedMove);


	public:
		/** Set the speed of the acceleration */
		inline void SetAccelerationSpeed(float NewSpeed) { mAccelerationSpeed = NewSpeed; }

		/** Get the acceleration speed */
		inline float GetAccelerationSpeed() const { return mAccelerationSpeed; }

		/** Set the speed of the deceleration */
		inline void SetDecelerationSpeed(float NewSpeed) { mDecelerationSpeed = NewSpeed; }

		/** Get the deceleration speed */
		inline float GetDecelerationSpeed() const { return mDecelerationSpeed; }

		/** Set the maximum movement speed */
		inline void SetMaxMovementSpeed(float NewSpeed) { mMaxMovementSpeed = NewSpeed; }

		/** Get the maximum movement speed */
		inline float GetMaxMovementSpeed() const { return mMaxMovementSpeed; }

		/** Set a value that represents the amount of slide when changing directions */
		void SetSlideFactor(float Factor);

		/** Get the value that represents the amount of slide when changing directions */
		inline float GetSlideFactor() const { return mSlideFactor; }

		/** Set the mass. A high mass makes the acceleration longer */
		void SetMass(float Mass);

		/** Get the mass */
		inline float GetMass() const { return mMass; }

		/** Get the current velocity */
		inline Vector<float> GetVelocity() const { return mVelocity; }

		/** Allow this component to perform the px calculation */
		inline void Enable() { bEnable = true; }

		/** Prevent this component to perform the px calculation */
		inline void Disable() { bEnable = false; }

		/** Called when the owner's collision component collides with another collision component */
		void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

	};
}