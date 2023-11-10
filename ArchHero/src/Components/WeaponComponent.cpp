#include "Components/WeaponComponent.h"
#include "Object.h"
#include "Components/AnimationComponent.h"
#include "Math/RotationMatrix.h"
#include "Components/BulletComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/BoxComponent.h"
#include "Math/RotationMatrix.h"

WeaponComponent::WeaponComponent(Object* InOwner) :
	SpriteComponent(InOwner)
{
}

void WeaponComponent::Update(float DeltaTime)
{
	if (!mInstigator || !mOwner) return;
	
	UpdateFlip();
	UpdateWeaponLocation();
	UpdateRotation();

	if (Input().IsButtonDown(0))
	{
		SpawnBullet();
	}
}

void WeaponComponent::Draw()
{
	SpriteComponent::Draw();
}

void WeaponComponent::UpdateFlip()
{
	// calculate angle between target and instigator (player)
	float AngleBetweentargetAndInstigator = mDirFromInstigator.GetNormalized().GetRotationFromX();
	float AngleDeg = (float)(AngleBetweentargetAndInstigator * 180 / PI);
	if (AngleDeg < 90)
	{
		mFlip.V = false;
		mInstigator->SetFlip(Flip(false, false));
	}
	else
	{
		mFlip.V = true;
		mInstigator->SetFlip(Flip(true, false));
	}
}

void WeaponComponent::UpdateWeaponLocation()
{
	// Put the weapon to the left or right of its instigator dependinf on its Flip

	Vector<float> InstigatorLoc = mInstigator->GetLocation();
	Vector<float> InstigatorSize = mInstigator->GetSize();
	Vector<float> OwnerSize = mOwner->GetSize();

	float OffsetX = mFlip.V || mFlip.H ? -mOffset.X : mOffset.X;
	Vector<float> InstigatorCenter = Vector<float>(InstigatorLoc.X + InstigatorSize.X / 2, InstigatorLoc.Y + InstigatorSize.Y / 2);
	Vector<float> AdjustedPos = Vector<float>(InstigatorCenter.X - OwnerSize.X / 2 + OffsetX, InstigatorCenter.Y + mOffset.Y);

	mOwner->SetLocation(AdjustedPos);
}

void WeaponComponent::UpdateRotation()
{
	if (!mTargetCursor) return;

	Vector<float> TargetCenter = Vector<float>(
		mTargetCursor->GetLocation().X + mTargetCursor->GetSize().X / 2,
		mTargetCursor->GetLocation().Y + mTargetCursor->GetSize().Y / 2
	);

	Vector<float> OwnerCenter = Vector<float>(
		mOwner->GetLocation().X + mOwner->GetSize().X / 2,
		mOwner->GetLocation().Y + mOwner->GetSize().Y / 2
	);

	Vector<float> InstigatorCenter = Vector<float>(
		mInstigator->GetLocation().X + mInstigator->GetSize().X / 2,
		mInstigator->GetLocation().Y + mInstigator->GetSize().Y / 2
	);

	mDirFromOwner = TargetCenter - OwnerCenter;
	mDirFromInstigator = TargetCenter - InstigatorCenter;

	float AngleRad = mDirFromOwner.GetNormalized().GetRotationFromX();
	float AngleDeg = (float)(AngleRad * 180 / PI);

	if (TargetCenter.Y < OwnerCenter.Y)
	{
		AngleDeg = 360 - AngleDeg;
	}

	mOwner->SetRotation(AngleDeg);
}

Object* WeaponComponent::SpawnBullet()
{
	// Rotate Src rect of weapon by its rotation
	RotationMatrix R;
	Vector<float> Points[4]{ Vector<float>::ZeroVector() };
	Rect<float> DstF = { static_cast<float>(mDst.X), static_cast<float>(mDst.Y), static_cast<float>(mDst.W), static_cast<float>(mDst.H) };
	R.RotateRect(mOwner->GetRotation(), DstF, Points);
	Rect<float> RotatedRect{Points[0].X, Points[0].Y, Points[1].X - Points[0].X, Points[2].Y - Points[0].Y};

	// Get the mid point of the rotated rect
	Vector<float> Mid = Vector<float>::ZeroVector();
	for (int i = 0; i < 4; i++)
	{
		Mid.X += Points[i].X;
		Mid.Y += Points[i].Y;
	}
	Mid.X /= 4;
	Mid.Y /= 4;

	// spawn bullet
	Object* Bullet = new Object();
	Bullet->AddTag("Bullet");
	Bullet->SetSize(20.f, 20.f);
	Bullet->SetRotation(-mOwner->GetRotation());
	Bullet->SetCenterLocation(Mid + mOwner->GetForwardVector() * 45);
	BulletComponent* BulletComp = Bullet->AddComponent<BulletComponent>();

	// Set texture file
	SpriteComponent* Sprite = Bullet->AddComponent<SpriteComponent>();
	std::string File = ASSET_PATH + std::string("Weapons/bullet.png");
	Sprite->SetFile(File);

	// Px
	PhysicComponent* PxComp = Bullet->AddComponent<PhysicComponent>();
	PxComp->SetMass(1.f);
	PxComp->SetSlideFactor(0);
	PxComp->SetMaxMovementSpeed(750);
	PxComp->SetAccelerationSpeed(10000);
	PxComp->SetDecelerationSpeed(10000);

	// Collision
	BoxComponent* BoxComp = Bullet->AddComponent<BoxComponent>();
	BoxComp->SetCollisionChannel(ECollisionChannel::Projectile);
	BoxComp->AddCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Overlap);
	BoxComp->SetBoxSize(20.f, 20.f);
	BoxComp->SetOffset(0, 0);

	// Add bullet to the world
	World().AddObject(Bullet);

	return Bullet;
}
