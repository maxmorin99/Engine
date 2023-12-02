#pragma once

#include "Component.h"
#include "Interfaces/IUpdatable.h"
#include "Interfaces/IObserver.h"
#include "Object.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace Core
{
	class IInput;
	class AnimationComponent;
	class PhysicComponent;
	class CollisionComponent;

	class PlayerComponent : public Component, public IUpdatable, public IObserver<std::unordered_map<std::string, void*>>
	{
	public:
		PlayerComponent(Object* Owner);
		virtual ~PlayerComponent() = default;
		virtual void Start();
		virtual void Destroy();
		virtual Component* Clone(Object* Owner);
		virtual void SetupClone(Component* Child);
		virtual void Update(float DeltaTime) override;

		/** Called when the player takes damage */
		virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

		/** Add hit sound effects
		*	@param InId: list of ids associated with hit sfx
		*/
		void AddHitSoundId(const std::vector<size_t>& InId);

		/** Set the id of the death sfx
		*	@param InId: Id associated with the death sfx
		*/
		void SetDeathSoundId(size_t InId);

	private:
		AnimationComponent* mAnimationComponent = nullptr;
		PhysicComponent* mPxComponent = nullptr;
		Vector<float> mCurrentVelocity = Vector<float>::ZeroVector();

		size_t mDeathSoundId = 0;
		std::vector<size_t> mHitSoundId;

		bool mWantToRoll = false;
		bool mRolling = false;
		bool bAlive = true;

		/** Get the movement inputs of the keybord 
		*	@param OutMovementInput: 2d axes representing keys pressed
		*/
		void GetMovementInput(Vector<float>& OutMovementInput);

		/** Called when the roll animation ends playing */
		void RollEndNotify();

		/** Called when the death animation ends playing */
		void DeathEndNotify();
		bool CheckReferences() const;

		/** Update the owner flip based on the owner's velocity */
		void UpdateFlip();

		/** Check if the owner wants to roll */
		void CheckRoll();

		/** Adjust the boc collision size from the owner size and readjust the collision location */
		void AdjustBoxSize();

		/** Called when the owner dies */
		void Die();
		
	public:

		/** Get the id associated with the death sfx */
		inline size_t GetDeathSondId() const { return mDeathSoundId; }
	};
}