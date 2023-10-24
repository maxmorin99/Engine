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

	Vector<float> TestDir[2] = { Vector<float>::ZeroVector() };
	

	Vector<float> mOffset = Vector<float>::ZeroVector();

	void UpdateFlip();
	void UpdateWeaponLocation();
	void UpdateRotation();
	float Dot(const Vector<float>& Vec1, const Vector<float>& Vec2);
	
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