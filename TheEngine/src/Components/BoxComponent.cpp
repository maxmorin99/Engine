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

	if (mOwner->HasTag("Player"))
	{
		int bob = 0;
	}

	Vector<float> OwnerLoc = mOwner->GetLocation();
	mRect.X = OwnerLoc.X + mOwnerOffset.X;
	mRect.Y = OwnerLoc.Y + mOwnerOffset.Y;

}

void Core::BoxComponent::Update(float DeltaTime)
{
	/*if (!mOwner) return;

	Vector<float> OwnerLoc = mOwner->GetLocation();
	mRect.X = OwnerLoc.X + mOwnerOffset.X;
	mRect.Y = OwnerLoc.Y + mOwnerOffset.Y;*/
}

void Core::BoxComponent::Draw()
{
	Graphic().DrawRectF(false, &mRect, Color::Red);
	if (mOwner->HasTag("Player"))
	{
		Graphic().DrawLineF(Vector<float>::ZeroVector(), mCollisionPoint, Color::Green);
	}
}

void Core::BoxComponent::SetCollisionLocation(const Vector<float>& NewLoc)
{
	CollisionComponent::SetCollisionLocation(NewLoc);

	mRect.X = NewLoc.X + mOwnerOffset.X;
	mRect.Y = NewLoc.Y + mOwnerOffset.Y;
}

void Core::BoxComponent::OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp, Vector<float>& CollisionPoint)
{
	CollisionComponent::OnCollisionHit(OtherObject, OtherComp, CollisionPoint);

	// replace owner where it was
	if (!mOwner) return;

	//mOwner->UseOldLocation();
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
