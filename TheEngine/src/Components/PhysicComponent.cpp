#include "Components/PhysicComponent.h"
#include "Object.h"
#include <algorithm>
#include "Components/BoxComponent.h"


Core::PhysicComponent::PhysicComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PhysicComponent::Start()
{
	if (mOwner)
	{
		mCollisionComponent = mOwner->GetCollisionComponent();
		if (mCollisionComponent)
		{
			mCollisionComponent->BindOnCollisionHit(this);
		}
	}
}

void Core::PhysicComponent::Update(float DeltaTime)
{
	if (!mOwner) return;

	if (bCollisionOccured)
	{
		ECollisionSide Test = GetCollisionNearestSide();
		int bob = 0;
	}

	// Si aucune collision, bouger le joueur à l'endroit de sa collision
	if (!bCollisionOccured && mPendingMove != Vector<float>::ZeroVector())
	{
		mOwner->SetLocation(mPendingMove);
		mOldLocation = mOwner->GetLocation();
	}
	else // sinon, remettre sa collision ou elle était avant
	{
		mCollisionComponent->SetCollisionLocation(mOldLocation);
		bCollisionOccured = false;
	}
	


	///////// Calculate pending move ///////////////////////////////

	mVelocity.X += mDesiredMoveThisFrame.X * DeltaTime;
	mVelocity.Y -= mDesiredMoveThisFrame.Y * DeltaTime;

	// keep the velocity <= max movement speed
	if (mVelocity.Length() > mMaxMovementSpeed)
	{
		mVelocity = mVelocity.GetNormalized() * mMaxMovementSpeed;
	}

	Vector<float> OwnerPos = mOwner->GetLocation();
	Vector<float> NewPos = OwnerPos + mVelocity * DeltaTime;
	mPendingMove = NewPos;
	
	if (mCollisionComponent)
	{
		if (mPendingMove != Vector<float>::ZeroVector())
		{
			// Add pending move to the collision to calculate collision
			mCollisionComponent->SetCollisionLocation(mPendingMove);
		}
	}

	//mOwner->SetLocation(NewPos);

	// Deceleration
	if (mDesiredMoveThisFrame.X == 0.f)
	{
		if (mVelocity.X < 0.f)
		{
			mVelocity.X += mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.X = std::clamp(mVelocity.X, -10000.f, 0.f);
		}
		else if (mVelocity.X > 0.f)
		{
			mVelocity.X -= mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.X = std::clamp(mVelocity.X, 0.f, 100000.f);
		}
	}
	if (mDesiredMoveThisFrame.Y == 0.f)
	{
		if (mVelocity.Y < 0.f)
		{
			mVelocity.Y += mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.Y = std::clamp(mVelocity.Y, -10000.f, 0.f);
		}
		else if (mVelocity.Y > 0.f)
		{
			mVelocity.Y -= mDecelerationSpeed * DeltaTime / mSlideFactor;
			mVelocity.Y = std::clamp(mVelocity.Y, 0.f, 10000.f);
		}
	}

	mDesiredMoveThisFrame = Vector<float>::ZeroVector();
}

void Core::PhysicComponent::AddMovement(const Vector<float>& MovementDirection)
{
	if (MovementDirection.Length() == 0.f) return;
	const Vector<float> NormalizedMovementDirection = MovementDirection.GetNormalized();
	mDesiredMoveThisFrame = NormalizedMovementDirection * mAccelerationSpeed / (mMass * mSlideFactor);
}

void Core::PhysicComponent::SetSlideFactor(float Factor)
{
	mSlideFactor = Factor == 0.f ? 1.f : Factor;
}

void Core::PhysicComponent::SetMass(float Mass)
{
	mMass = Mass == 0.f ? 1.f : Mass;
}

void Core::PhysicComponent::OnNotify(const T1& Value1, const T2& Value2, const T3& Value3)
{
	bCollisionOccured = true;
}

Core::ECollisionSide Core::PhysicComponent::GetCollisionNearestSide()
{
	BoxComponent* Box = dynamic_cast<BoxComponent*>(mCollisionComponent);
	if(!Box) return ECollisionSide();

	Vector<float> LeftSide[2]{
		Vector<float>(Box->GetRect().X, Box->GetRect().Y),
		Vector<float>(Box->GetRect().X, Box->GetRect().Y + Box->GetRect().H)
	};
	Vector<float> RightSide[2]{
		Vector<float>(Box->GetRect().X + Box->GetRect().W, Box->GetRect().Y),
		Vector<float>(Box->GetRect().X + Box->GetRect().W, Box->GetRect().Y + Box->GetRect().H)
	};
	Vector<float> TopSide[2]{
		Vector<float>(Box->GetRect().X, Box->GetRect().Y),
		Vector<float>(Box->GetRect().X + Box->GetRect().W, Box->GetRect().Y)
	};
	Vector<float> BotSide[2]{
		Vector<float>(Box->GetRect().X, Box->GetRect().Y + Box->GetRect().H),
		Vector<float>(Box->GetRect().X + Box->GetRect().W, Box->GetRect().Y + Box->GetRect().H)
	};
	
	// find nearest side from mCollisionPoint

	float DistWithTop = DistancePointToLine(TopSide);
	float DistWithBot = DistancePointToLine(BotSide);
	float DistWithLeft = DistancePointToLine(LeftSide);
	float DistWithRight = DistancePointToLine(RightSide);

	if (DistWithTop < DistWithBot && DistWithTop < DistWithLeft && DistWithTop < DistWithRight)
	{
		return ECollisionSide::Top;
	}
	if (DistWithBot < DistWithTop && DistWithBot < DistWithLeft && DistWithBot < DistWithRight)
	{
		return ECollisionSide::Bot;
	}
	if (DistWithLeft < DistWithTop && DistWithLeft < DistWithBot && DistWithLeft < DistWithRight)
	{
		return ECollisionSide::Left;
	}
	return ECollisionSide::Right;
}

float Core::PhysicComponent::DistancePointToLine(Vector<float> Line[2])
{
	float LineLength = Vector<float>::Dist(Line[0], Line[1]);

	float t = ((mCollisionPoint.X - Line[0].X) * (Line[1].X - Line[0].X) + (mCollisionPoint.Y - Line[0].Y) * (Line[1].Y - Line[0].Y)) / (LineLength * LineLength);
	t = std::max(0.0f, std::min(1.0f, t));

	float NearestX = Line[0].X + t * (Line[1].X - Line[0].X);
	float NearestY = Line[0].Y + t * (Line[1].Y - Line[0].Y);

	// Calcul de la distance entre le point et le point le plus proche sur la ligne.
	return Vector<float>::Dist(Vector<float>(mCollisionPoint.X, mCollisionPoint.Y), {NearestX, NearestY});
}

void Core::PhysicComponent::Destroy()
{
}
