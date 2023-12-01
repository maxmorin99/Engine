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
		virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

		void AddHitSoundId(const std::vector<size_t>& InId);
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

		void GetMovementInput(Vector<float>& OutMovementInput);
		void RollEndNotify();
		void DeathEndNotify();
		bool CheckReferences() const;
		void UpdateFlip();
		void CheckRoll();
		void AdjustBoxSize();
		void Die();
		
	public:
		inline size_t GetDeathSondId() const { return mDeathSoundId; }
	};
}