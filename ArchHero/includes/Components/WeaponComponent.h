#pragma once

#include "Components/SpriteComponent.h"
#include "Interfaces/IUpdatable.h"

using namespace Core;

class WeaponComponent : public SpriteComponent, public IUpdatable
{
public:
	WeaponComponent(Object* InOwner);
	
	virtual void Update(float DeltaTime) override;
	virtual void Draw() override;

private:
	Object* mInstigator = nullptr;

	Vector<float> Test[4] = { Vector<float>::ZeroVector() };
	Vector<float> MiddlePoint = Vector<float>::ZeroVector();
	
	// TODO ref to the bullet class

public:
	/** Set ref to the Object that posses this weapon. Ex: The player/AI that uses this weapon */
	inline void SetInstigator(Object* InInstigator) { mInstigator = InInstigator; }
	/** Get ref to the Object that posses this weapon. Ex: The player/AI that uses this weapon */
	inline Object* GetInstigator() const { return mInstigator; }
};