#include "Components/WeaponComponent.h"
#include "Object.h"
#include "Components/AnimationComponent.h"
#include "Math/RotationMatrix.h"

WeaponComponent::WeaponComponent(Object* InOwner) :
	SpriteComponent(InOwner)
{
}

void WeaponComponent::Update(float DeltaTime)
{
	if (!mInstigator || !mOwner) return;


	Vector<float> InstigatorLoc = mInstigator->GetLocation();
	Vector<float> InstigatorSize = mInstigator->GetSize();
	Vector<float> OwnerSize = mOwner->GetSize();

	mFlip = mInstigator->GetComponent<AnimationComponent>()->GetFlip();
	bool TempFlipH = mFlip.H;
	mFlip.H = mFlip.V;
	mFlip.V = TempFlipH;
	float OffsetX = mFlip.V ? -30 : +30;

	Vector<float> InstigatorCenter = Vector<float>(InstigatorLoc.X + InstigatorSize.X / 2, InstigatorLoc.Y + InstigatorSize.Y / 2);
	Vector<float> AdjustedPos = Vector<float>(InstigatorCenter.X - OwnerSize.X / 2 + OffsetX, InstigatorCenter.Y + 50);

	mOwner->SetLocation(AdjustedPos);
	int CurrRot = mOwner->GetRotation();
	CurrRot += 100 * DeltaTime;
	CurrRot = CurrRot % 360;
	mOwner->SetRotation(CurrRot);

	RotationMatrix R;
	Rect<float> OwnerRect;
	OwnerRect.X = mOwner->GetLocation().X;
	OwnerRect.Y = mOwner->GetLocation().Y;
	OwnerRect.W = mOwner->GetSize().X;
	OwnerRect.H = mOwner->GetSize().Y;
	R.RotateRect(CurrRot, OwnerRect, Test);
	MiddlePoint.X = (Test[1].X + Test[3].X) / 2;
	MiddlePoint.Y = (Test[1].Y + Test[3].Y) / 2;
}

void WeaponComponent::Draw()
{
	SpriteComponent::Draw();

	Vector<float> Fwd = mOwner->GetForwardVector();
	Graphic().DrawLineF(Test[0], Test[1], Color::Red);
	Graphic().DrawLineF(Test[1], Test[3], Color::Red);
	Graphic().DrawLineF(Test[3], Test[2], Color::Red);
	Graphic().DrawLineF(Test[2], Test[0], Color::Red);
	Graphic().DrawLineF(MiddlePoint, MiddlePoint + mOwner->GetForwardVector() * 100, Color::Red);
}
