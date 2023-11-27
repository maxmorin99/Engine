#include "Components/WeaponComponent.h"
#include "Object.h"
#include "Components/AnimationComponent.h"
#include "Math/RotationMatrix.h"
#include "Components/BulletComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AnimationComponent.h"
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
	UpdateFire(DeltaTime);
}

void WeaponComponent::Draw()
{
	SpriteComponent::Draw();
}

void WeaponComponent::Start()
{
	SpriteComponent::Start();

	DefineBulletPrototype();

	mFireTimeElapsed = mFireRate;
}

Component* WeaponComponent::Clone(Object* Owner)
{
	WeaponComponent* Clone = new WeaponComponent(Owner);
	__super::SetupClone(Clone);

	return Clone;
}

void WeaponComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);
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

void WeaponComponent::UpdateFire(float DeltaTime)
{
	mFireTimeElapsed += DeltaTime;
	if (Input().IsButtonDown(0))
	{
		if (mFireTimeElapsed > mFireRate)
		{
			mFireTimeElapsed = 0.f;
			SpawnBullet();
		}
	}
}

void WeaponComponent::DefineBulletPrototype()
{
	Object* Bullet = new Object();
	Bullet->AddTag("Bullet");
	Bullet->SetSize(20.f, 20.f);
	Bullet->SetRotation(-mOwner->GetRotation());
	BulletComponent* BulletComp = Bullet->AddComponent<BulletComponent>();

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
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Overlap);
	BoxComp->SetBoxSize(20.f, 20.f);

	// Animation
	AnimationComponent* BulletAnim = Bullet->AddComponent<AnimationComponent>();
	std::string ExplosionFile = ASSET_PATH + std::string("Projectiles/PlayerProjectile.png");
	BulletAnim->SetFile(ExplosionFile);
	std::vector<Frame> BulletIdleFrames;
	BulletIdleFrames.push_back(Frame(3, 0, 256, 256, "Idle_1"));
	BulletIdleFrames.push_back(Frame(0, 1, 256, 256, "Idle_2"));
	BulletIdleFrames.push_back(Frame(1, 1, 256, 256, "Idle_3"));
	BulletIdleFrames.push_back(Frame(2, 1, 256, 256, "Idle_4"));
	BulletIdleFrames.push_back(Frame(3, 1, 256, 256, "Idle_5"));
	Clip IdleClip("IdleClip", BulletIdleFrames, 0.1f);
	BulletAnim->AddClip("IdleClip", IdleClip);

	std::vector<Frame> BulletExplosionFrames;
	BulletExplosionFrames.push_back(Frame(4, 0, 256, 256, "Explosion_1"));
	BulletExplosionFrames.push_back(Frame(5, 0, 256, 256, "Explosion_2"));
	BulletExplosionFrames.push_back(Frame(6, 0, 256, 256, "Explosion_3"));
	BulletExplosionFrames.push_back(Frame(7, 0, 256, 256, "Explosion_4"));
	Clip ExplosionClip("ExplosionClip", BulletExplosionFrames, 0.005f);
	BulletAnim->AddClip("ExplosionClip", ExplosionClip);

	BulletAnim->SetDefaultClip(IdleClip);
	BulletAnim->SetClip("IdleClip", true);

	Spawner().AddPrototype("Bullet", Bullet);
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

	Object* Bullet = Spawner().Spawn("Bullet");
	Bullet->SetRotation(-mOwner->GetRotation());
	Bullet->SetCenterLocation(Mid + mOwner->GetForwardVector() * 45);

	// Add bullet to the world
	World().AddObject(Bullet);

	return Bullet;
}

