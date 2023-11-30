#pragma once

#include "Component.h"
#include "Subject.h"
#include <unordered_map>
#include <string>

namespace Core
{
	class AttributeComponent : public Component
	{
	public:
		AttributeComponent(Object* Owner);
		virtual ~AttributeComponent() = default;
		virtual Component* Clone(Object* Owner) override;
		virtual void SetupClone(Component* Child) override;
		virtual void Start() override;

		Subject<std::unordered_map<std::string, void*>> mOnHealthChangedSubject;

	private:
		float mMaxHealth = 100.f;
		float mCurrentHealth = 100.f;

	public:
		inline void SetMaxHealth(float InHealth) { mMaxHealth = InHealth; }

		/** return percent of the object's health from 0 -> 1, 1 = 100% */
		float GetPercentHealth() const;
		void TakeDamage(float InDamage);
	};
}