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
	Object* mTargetCursor = nullptr;
	Vector<float> mDirFromOwner = Vector<float>::ZeroVector();
	Vector<float> mDirFromInstigator = Vector<float>::ZeroVector();

	/** Offset from instigator */
	Vector<float> mOffset = Vector<float>::ZeroVector();

	/** Update weapon and Instigator's flip depending on the weapon's rotation */
	void UpdateFlip();

	/** Update the weapon location depending on its rotation */
	void UpdateWeaponLocation();
	void UpdateRotation();
	
	// TODO ref to the bullet class


public:
	/** Set ref to the Object that posses this weapon. Ex: The player/AI that uses this weapon */
	inline void SetInstigator(Object* InInstigator) { mInstigator = InInstigator; }

	/** Get ref to the Object that posses this weapon. Ex: The player/AI that uses this weapon */
	inline Object* GetInstigator() const { return mInstigator; }

	inline void SetTargetCursorObject(Object* InTargetCursor) { mTargetCursor = InTargetCursor; }

	/** Offset from the instigator to put the weapon to the right, to the left, ... */
	inline void SetOffset(const Vector<float>& InOffset) { mOffset = InOffset; }
};