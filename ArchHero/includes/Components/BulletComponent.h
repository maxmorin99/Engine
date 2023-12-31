#pragma once

#include "Components/SpriteComponent.h"
#include "Interfaces/IUpdatable.h"
#include "Interfaces/IObserver.h"
#include <vector>

using namespace Core;

class BulletComponent : public SpriteComponent, public IUpdatable, public IObserver<std::unordered_map<std::string, void*>>
{
public:
	BulletComponent(Object* Owner);
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;
	virtual Component* Clone(Object* Owner) override;
	virtual void SetupClone(Component* Child) override;

	virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

	void AddImpactWallSoundId(const std::vector<size_t>& InId);
	void AddImpactBodySoundId(const std::vector<size_t>& InId);

private:
	PhysicComponent* mPxComp = nullptr;

	std::vector<size_t> mImpactWallSoundIdList;
	std::vector<size_t> mImpactBodySoundIdList;

	/** Damage that this bullet causes */
	float mDamage = 10.f;

	void DisableCollision();

	void DisablePhysic();

	/** Play a specific destroy animation. The animation will call OnExplosionEnd when done */
	void PlayDestroyAnimation(const std::string& AnimationName);

	/** Called when the destroy animation end playing */
	void OnExplosionEnd();

public:
	inline void SetDamage(float InDamage) { mDamage = InDamage; }
};


