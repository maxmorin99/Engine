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
		Subject<std::unordered_map<std::string, void*>> mOnDeathSubject;

	private:
		float mMaxHealth = 100.f;
		float mCurrentHealth = 100.f;

	public:
		void SetCurrentHealth(float InHealth);
		inline void SetMaxHealth(float InHealth) { mMaxHealth = InHealth; mCurrentHealth = InHealth; }
		inline float GetMaxHealth() const { return mMaxHealth; }

		/** return percent of the object's health from 0 -> 100 */
		float GetPercentHealth() const;

		/** Reduces the health attribute by the amount defined bu InDamage */
		void TakeDamage(float InDamage);
	};
}