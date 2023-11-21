#include "Components/BoxComponent.h"
#include "Object.h"

Core::BoxComponent::BoxComponent(Object* Owner) :
	CollisionComponent(Owner)
{
	mCollisionType = ECollisionShape::Rectangle;
}

void Core::BoxComponent::Start()
{
	CollisionComponent::Start();

	if (!mOwner) return;

	Vector<float> OwnerLoc = mOwner->GetLocation();
	mRect.X = OwnerLoc.X + mOwnerOffset.X;
	mRect.Y = OwnerLoc.Y + mOwnerOffset.Y;
}

void Core::BoxComponent::Update(float DeltaTime)
{
	CollisionComponent::Update(DeltaTime);
}

void Core::BoxComponent::Draw()
{
	/*Graphic().DrawRectF(false, &mRect, Color::Red);
	Vector<float> OwnerSize = mOwner->GetSize();
	Vector<float> OwnerSpriteCenterOffset = mOwner->GetCenterOffset();
	Vector<float> Center = Vector<float>(mOwner->GetLocation().X + OwnerSize.X / 2 + OwnerSpriteCenterOffset.X, mOwner->GetLocation().Y + OwnerSize.Y / 2 + OwnerSpriteCenterOffset.Y);
	Graphic().DrawLineF(Vector<float>::ZeroVector(), Center, Color::Green);*/
}

void Core::BoxComponent::SetCollisionLocation(const Vector<float>& NewLoc)
{
	CollisionComponent::SetCollisionLocation(NewLoc);

	mRect.X = NewLoc.X + mOwnerOffset.X;
	mRect.Y = NewLoc.Y + mOwnerOffset.Y;
}

Core::Vector<float> Core::BoxComponent::GetCollisionLocation() const
{
	return Vector<float>(mRect.X - mOwnerOffset.X, mRect.Y - mOwnerOffset.Y);
}

void Core::BoxComponent::OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp)
{
	CollisionComponent::OnCollisionHit(OtherObject, OtherComp);
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
