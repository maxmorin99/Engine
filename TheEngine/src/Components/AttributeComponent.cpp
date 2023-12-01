#include "Components/AttributeComponent.h"

Core::AttributeComponent::AttributeComponent(Object* Owner) :
	Component(Owner)
{
}

Core::Component* Core::AttributeComponent::Clone(Object* Owner)
{
	AttributeComponent* Clone = new AttributeComponent(Owner);
	__super::SetupClone(Clone);

	Clone->mMaxHealth = mMaxHealth;
	Clone->mCurrentHealth = mMaxHealth;

	return Clone;
}

void Core::AttributeComponent::SetupClone(Component* Child)
{
	AttributeComponent* Clone = dynamic_cast<AttributeComponent*>(Child);
	__super::SetupClone(Child);

	if (Clone)
	{
		Clone->mMaxHealth = mMaxHealth;
		Clone->mCurrentHealth = mMaxHealth;
	}
}

void Core::AttributeComponent::Start()
{
	__super::Start();
	
}

void Core::AttributeComponent::SetCurrentHealth(float InHealth)
{
	mCurrentHealth = InHealth;
	float NewHealthPercent = GetPercentHealth();
	std::unordered_map<std::string, void*> ParamsMap;
	ParamsMap["Health"] = &mCurrentHealth;
	ParamsMap["MaxHealth"] = &mMaxHealth;
	ParamsMap["Percent"] = &NewHealthPercent;
	mOnHealthChangedSubject.Invoke(ParamsMap);
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
	float NewCurrHealth = mCurrentHealth;
	float NewHealthPercent = GetPercentHealth();

	if (OldCurrHealth != NewCurrHealth)
	{
		std::unordered_map<std::string, void*> ParamsMap;
		ParamsMap["Health"] = &mCurrentHealth;
		ParamsMap["MaxHealth"] = &mMaxHealth;
		ParamsMap["Percent"] = &NewHealthPercent;
		mOnHealthChangedSubject.Invoke(ParamsMap);

		if (NewHealthPercent == 0.f)
		{
			std::unordered_map<std::string, void*> mParamsMap;
			mOnDeathSubject.Invoke(mParamsMap);
		}
	}
}
