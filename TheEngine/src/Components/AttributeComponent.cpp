#include "Components/AttributeComponent.h"

Core::AttributeComponent::AttributeComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::AttributeComponent::Start()
{
	__super::Start();

	mCurrentHealth = mMaxHealth;
}

float Core::AttributeComponent::GetPercentHealth() const
{
	if (mMaxHealth == 0) return 0.f;
	return mCurrentHealth / mMaxHealth * 100;
}

void Core::AttributeComponent::TakeDamage(float InDamage)
{
	float OldCurrHealth = mCurrentHealth;
	mCurrentHealth -= InDamage;
	mCurrentHealth = mCurrentHealth > 0.f ? mCurrentHealth : 0.f;
	float NewCurrhealth = mCurrentHealth;
	float NewHealthPercent = GetPercentHealth();

	if (OldCurrHealth != NewCurrhealth)
	{
		std::unordered_map<std::string, void*> ParamsMap;
		ParamsMap["Health"] = &mCurrentHealth;
		ParamsMap["MaxHealth"] = &mMaxHealth;
		ParamsMap["Percent"] = &NewHealthPercent;
		mOnHealthChangedSubject.Invoke(ParamsMap);
	}

	// make object die
}
