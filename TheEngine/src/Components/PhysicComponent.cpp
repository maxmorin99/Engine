#include "Components/PhysicComponent.h"
#include "Object.h"
#include <algorithm>
#include "Components/BoxComponent.h"
#include <math.h>


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

	// Si aucune collision, bouger le joueur à l'endroit de sa collision
	if (!bCollisionOccured && Vector<float>::Dist(mPendingMove, mOwner->GetLocation()) == 0.f)
	{
		mOwner->SetLocation(mPendingMove);
		mOldLocation = mOwner->GetLocation();
	}
	else // sinon, remettre sa collision ou elle était avant
	{
		ECollisionSide Side = GetCollisionNearestSide();
		Vector<float> CorrectedMove = Vector<float>::ZeroVector();
		
		switch (Side)
		{
		case ECollisionSide::Left:
			CorrectedMove = Vector<float>(mOldLocation.X, mPendingMove.Y);
			break;
		case ECollisionSide::Right:
			break;
		case ECollisionSide::Top:
			break;
		case ECollisionSide::Bot:
			break;
		default:
			break;
		}

		mOwner->SetLocation(CorrectedMove);
		mCollisionComponent->SetCollisionLocation(CorrectedMove);
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

void Core::PhysicComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	bCollisionOccured = true;
	
	if (Value.count("CollisionPoint") > 0)
	{
		void* CollisionPointPtr = Value.at("CollisionPoint");
		Vector<float>* CollisionPointVec = static_cast<Vector<float>*>(CollisionPointPtr);
		
		if (CollisionPointVec)
		{
			mCollisionPoint = *CollisionPointVec;
		}
	}
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
	Vector<float> ToSubstractToGetCenter{ abs(Line[1].X - Line[0].X) / 2, abs(Line[1].Y - Line[0].Y) / 2 };
	Vector<float> LineCenter{ std::max(Line[0].X, Line[1].X) - ToSubstractToGetCenter.X, std::max(Line[0].Y, Line[1].Y) - ToSubstractToGetCenter.Y };
	return Vector<float>::Dist(LineCenter, mCollisionPoint);
}

void Core::PhysicComponent::Destroy()
{
}
