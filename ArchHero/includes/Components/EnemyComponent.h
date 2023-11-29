#pragma once

#include "Components/Component.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IUpdatable.h"
#include <unordered_map>
#include <string>

using namespace Core;

class IState;

class EnemyComponent : public Component, public IDrawable, public IUpdatable
{
public:
	EnemyComponent(Object* Owner);
	virtual ~EnemyComponent() = default;
	virtual void Start() override;
	virtual void Destroy() override;
	virtual void Draw() override;
	virtual void Update(float DeltaTime) override;
	virtual Component* Clone(Object* Owner) override;
	virtual void SetupClone(Component* Child) override;
	void Attack();
	void ChangeState(const std::string& StateName);

private:
	PathFindingComponent* mPathFindingComp = nullptr;
	std::vector<Vector<float>> mPath;

	Object* mTarget = nullptr;

	bool bHasPath = false;

	float mToleranceDistance = 100.f;

	float mAttackDelay = 2.f;
	float mCurrAttackDelay = 0.f;

	std::unordered_map<std::string, IState*> mStates;
	IState* mCurrentState = nullptr;

	void UpdateAnimation();
	void UpdateFlip();

public:
	inline void SetToleranceDistance(float InDist) { mToleranceDistance = InDist; }
	inline Object* GetTarget() const { return mTarget; }
	inline Object* GetOwner() const { return mOwner; }
	inline float GetToleranceDistance() const { return mToleranceDistance; }
	inline void SetAttackDelay(float InDelay) { mAttackDelay = InDelay; }
	inline float GetAttackDelay() const { return mAttackDelay; }
	inline void SetCurrAttackDelay(float InDelay) { mCurrAttackDelay = InDelay; }
	inline float GetCurrAttackDelay() const { return mCurrAttackDelay; }
};