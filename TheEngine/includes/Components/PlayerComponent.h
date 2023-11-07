#pragma once

#include "Component.h"
#include "Interfaces/IUpdatable.h"
#include "Object.h"

namespace Core
{
	class IInput;
	class AnimationComponent;
	class PhysicComponent;
	class CollisionComponent;

	class PlayerComponent : public Component, public IUpdatable
	{
	public:
		PlayerComponent(Object* Owner);
		virtual ~PlayerComponent() = default;
		virtual void Start();
		virtual void Destroy();

		virtual void Update(float DeltaTime) override;

	private:
		AnimationComponent* mAnimationComponent = nullptr;
		PhysicComponent* mPxComponent = nullptr;

		bool mWantToRoll = false;
		bool mRolling = false;

		void GetMovementInput(Vector<float>& OutMovementInput);
		void RollEndNotify();
		bool CheckReferences() const;
	};
}