#include "Components/BoxComponent.h"
#include "Object.h"

Core::BoxComponent::BoxComponent(Object* Owner) :
	CollisionComponent(Owner)
{
	mCollisionType = ECollisionShape::Rectangle;
}

void Core::BoxComponent::Update(float DeltaTime)
{
	if (!mOwner) return;

	Vector<float> OwnerLoc = mOwner->GetLocation();
	mRect.X = OwnerLoc.X + mOwnerOffset.X;
	mRect.Y = OwnerLoc.Y + mOwnerOffset.Y;
}

void Core::BoxComponent::Draw()
{
	Graphic().DrawRectF(false, &mRect, Color::Red);
}

void Core::BoxComponent::SetOffset(float OffsetX, float OffsetY)
{
	mOwnerOffset.X = OffsetX;
	mOwnerOffset.Y = OffsetY;
}

void Core::BoxComponent::SetBoxSize(float SizeX, float SizeY)
{
	mRect.W = SizeX;
	mRect.H = SizeY;
}
