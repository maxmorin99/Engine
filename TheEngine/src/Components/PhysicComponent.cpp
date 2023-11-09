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

	/////////////////// Process movement based on registered collisions //////////////////////////////////////
	
	if (!bCollisionOccured)
	{
		// No collision: move player with pending move

		Vector<float> OwnerCurrentLoc = mOwner->GetLocation();
		mOwner->SetLocation(OwnerCurrentLoc + mPendingMove);
	}
	else 
	{
		// Move the player with a corrected move to simulate blocking collision

		Vector<float> CorrectedMove = Vector<float>::ZeroVector();
		MovementCorrection(CorrectedMove);
		mOwner->SetLocation(mOwner->GetLocation() + CorrectedMove);

		bCollisionOccured = false;
		mCollisionSides[0] = ECollisionSide::Undefined;
		mCollisionSides[1] = ECollisionSide::Undefined;
	}


	///////// Calculate pending move ///////////////////////////////

	UpdateVelocity(DeltaTime);

	Vector<float> OwnerPos = mOwner->GetLocation();
	mPendingMove = mVelocity * DeltaTime;
	
	ApplyPendingMoveToCollisionComponent();

	ApplyDeceleration(DeltaTime);

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
	
	if (Value.count("OtherComponent") > 0)
	{
		void* CompAnyPtr = Value.at("OtherComponent");
		CollisionComponent* CompPtr = static_cast<CollisionComponent*>(CompAnyPtr);
		if (CompPtr)
		{
			if (mCollisionSides[0] == ECollisionSide::Undefined)
			{
				mCollisionSides[0] = GetCollisionSideFromOtherComponent(CompPtr);
			}
			else
			{
				mCollisionSides[1] = GetCollisionSideFromOtherComponent(CompPtr);
			}
		}
	}
}

void Core::PhysicComponent::UpdateVelocity(float DeltaTime)
{
	mVelocity.X += mDesiredMoveThisFrame.X * DeltaTime;
	mVelocity.Y -= mDesiredMoveThisFrame.Y * DeltaTime;

	// keep the velocity <= max movement speed
	if (mVelocity.Length() > mMaxMovementSpeed)
	{
		mVelocity = mVelocity.GetNormalized() * mMaxMovementSpeed;
	}
}

void Core::PhysicComponent::ApplyDeceleration(float DeltaTime)
{
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
}

void Core::PhysicComponent::ApplyPendingMoveToCollisionComponent()
{
	Vector<float> OwnerPos = mOwner->GetLocation();

	if (mCollisionComponent)
	{
		if (mPendingMove != Vector<float>::ZeroVector())
		{
			// Add pending move to the collision to calculate collision
			mCollisionComponent->SetCollisionLocation(OwnerPos + mPendingMove);
		}
	}
}

Core::ECollisionSide Core::PhysicComponent::GetCollisionSideFromOtherComponent(CollisionComponent* CollisionComp)
{
	BoxComponent* OwnerBox = dynamic_cast<BoxComponent*>(mCollisionComponent);
	BoxComponent* Box = dynamic_cast<BoxComponent*>(CollisionComp);

	if (Box && OwnerBox)
	{
		Vector<float> OwnerBoxCenter{OwnerBox->GetRect().X + OwnerBox->GetRect().W / 2, OwnerBox->GetRect().Y + OwnerBox->GetRect().H / 2};
		Vector<float> BoxCenter{Box->GetRect().X + Box->GetRect().W / 2, Box->GetRect().Y + Box->GetRect().H / 2};
		Vector<float> Dir{BoxCenter.X - OwnerBoxCenter.X, BoxCenter.Y - OwnerBoxCenter.Y};
		Rect<float> OwnerRect = OwnerBox->GetRect();
		Rect<float> OtherRect = Box->GetRect();
		ECollisionSide OutCollisionSide;
		
		if (mPendingMove.X != 0 && mPendingMove.Y != 0)
		{
			// movement dans deux axes

			if (HandleTwoAxesInput(Dir, OwnerRect, OtherRect, OutCollisionSide))
			{
				return OutCollisionSide;
			}
		}

		if (mCollisionSides[0] == ECollisionSide::Undefined)
		{
			mCollisionRects[0] = OtherRect;
		}
		else
		{
			mCollisionRects[1] = OtherRect;
		}

		// movement sur un axe

		if (HandleOneAxisInput(Dir, OtherRect, OutCollisionSide))
		{
			return OutCollisionSide;
		}
	}

	return ECollisionSide::Undefined;
}

bool Core::PhysicComponent::ShouldProcessTopBotCollisionFirst(float Threshold, const Rect<float>& OwnerRect, const Rect<float>& OtherRect) const
{
	if (OwnerRect.Y < (OtherRect.Y + OtherRect.H) && OwnerRect.Y + Threshold >(OtherRect.Y + OtherRect.H) ||
		OwnerRect.Y + OwnerRect.H > OtherRect.Y && OwnerRect.Y + OwnerRect.H - Threshold < OtherRect.Y)
	{
		return true;
	}
	return false;
}

void Core::PhysicComponent::ReAdjustOwnerCollisionLocation(const ECollisionSide& CollisionSide)
{
	// A collision has been processed. Replace the collision 
	Vector<float> OwnerLoc = mOwner->GetLocation();
	BoxComponent* Box = dynamic_cast<BoxComponent*>(mCollisionComponent);
	if (Box)
	{
		if (CollisionSide == ECollisionSide::Top || CollisionSide == ECollisionSide::Bot)
		{
			Vector<float> CollisionLoc = Box->GetCollisionLocation();
			Box->SetCollisionLocation(Vector<float>(CollisionLoc.X, OwnerLoc.Y));
		}
		else
		{
			Vector<float> CollisionLoc = Box->GetCollisionLocation();
			Box->SetCollisionLocation(Vector<float>(OwnerLoc.X, CollisionLoc.Y));
		}
	}
}

bool Core::PhysicComponent::HandleTwoAxesInput(const Vector<float>& Dir, const Rect<float>& OwnerRect, const Rect<float>& OtherRect, ECollisionSide& OutCollisionSide)
{
	if (mCollisionSides[0] == ECollisionSide::Undefined)
	{
		// premiere collision détectée cette frame-ci

		mCollisionRects[0] = OtherRect;
		float Threshold = 10.f;

		if (ShouldProcessTopBotCollisionFirst(Threshold, OwnerRect, OtherRect))
		{
			// gérer top bot en premier

			if (mPendingMove.Y < 0 && Dir.Y < 0)
			{
				// top
				ReAdjustOwnerCollisionLocation(ECollisionSide::Top);

				OutCollisionSide = ECollisionSide::Top;
				return true;
			}
			else if (mPendingMove.Y > 0 && Dir.Y > 0)
			{
				// bot
				ReAdjustOwnerCollisionLocation(ECollisionSide::Bot);

				OutCollisionSide = ECollisionSide::Bot;
				return true;
			}
		}
		else
		{
			// gérer left right en premier

			if (mPendingMove.X > 0 && Dir.X > 0)
			{
				// droite
				ReAdjustOwnerCollisionLocation(ECollisionSide::Right);

				OutCollisionSide = ECollisionSide::Right;
				return true;
			}
			else if (mPendingMove.X < 0 && Dir.X < 0)
			{
				// gauche
				ReAdjustOwnerCollisionLocation(ECollisionSide::Left);

				OutCollisionSide = ECollisionSide::Left;
				return true;
			}
		}
	}
	else
	{
		// deuxieme collision détectée cette frame-ci

		mCollisionRects[1] = OtherRect;

		if (mCollisionSides[0] == ECollisionSide::Left || mCollisionSides[0] == ECollisionSide::Right)
		{
			// collision gauche droite déja géré

			if (mPendingMove.Y < 0 && Dir.Y < 0)
			{
				// top
				ReAdjustOwnerCollisionLocation(ECollisionSide::Top);

				OutCollisionSide = ECollisionSide::Top;
				return true;
			}
			else if (mPendingMove.Y > 0 && Dir.Y > 0)
			{
				// bot
				ReAdjustOwnerCollisionLocation(ECollisionSide::Bot);

				OutCollisionSide = ECollisionSide::Bot;
				return true;
			}
		}
		else if (mCollisionSides[0] == ECollisionSide::Top || mCollisionSides[0] == ECollisionSide::Bot)
		{
			// Collision top bot déja géré

			if (mPendingMove.X > 0 && Dir.X > 0)
			{
				// droite
				ReAdjustOwnerCollisionLocation(ECollisionSide::Right);

				OutCollisionSide = ECollisionSide::Right;
				return true;
			}
			else if (mPendingMove.X < 0 && Dir.X < 0)
			{
				// gauche
				ReAdjustOwnerCollisionLocation(ECollisionSide::Left);

				OutCollisionSide = ECollisionSide::Left;
				return true;
			}
		}
	}
	return false;
}

bool Core::PhysicComponent::HandleOneAxisInput(const Vector<float>& Dir, const Rect<float>& OtherRect, ECollisionSide& OutCollisionSide)
{
	if (mPendingMove.X > 0 && Dir.X > 0)
	{
		// droite
		OutCollisionSide = ECollisionSide::Right;
		return true;
	}
	else if (mPendingMove.X < 0 && Dir.X < 0)
	{
		// gauche
		OutCollisionSide = ECollisionSide::Left;
		return true;
	}

	if (mPendingMove.Y < 0 && Dir.Y < 0)
	{
		// top
		OutCollisionSide = ECollisionSide::Top;
		return true;
	}
	else if (mPendingMove.Y > 0 && Dir.Y > 0)
	{
		// bot
		OutCollisionSide = ECollisionSide::Bot;
		return true;
	}

	OutCollisionSide = ECollisionSide::Undefined;
	return false;
}

void Core::PhysicComponent::MovementCorrection(Vector<float>& CorrectedMove)
{
	if (mCollisionSides[0] != ECollisionSide::Undefined)
	{
		switch (mCollisionSides[0])
		{
		case ECollisionSide::Left:
			CorrectedMove = Vector<float>(CorrectedMove.X, mPendingMove.Y);
			break;
		case ECollisionSide::Right:
			CorrectedMove = Vector<float>(CorrectedMove.X, mPendingMove.Y);
			break;
		case ECollisionSide::Top:
			CorrectedMove = Vector<float>(mPendingMove.X, CorrectedMove.Y);
			break;
		case ECollisionSide::Bot:
			CorrectedMove = Vector<float>(mPendingMove.X, CorrectedMove.Y);
			break;
		}
	}
	if (mCollisionSides[1] != ECollisionSide::Undefined)
	{
		switch (mCollisionSides[1])
		{
		case ECollisionSide::Left:
			CorrectedMove = Vector<float>(0, CorrectedMove.Y);
			break;
		case ECollisionSide::Right:
			CorrectedMove = Vector<float>(0, CorrectedMove.Y);
			break;
		case ECollisionSide::Top:
			CorrectedMove = Vector<float>(CorrectedMove.X, 0);
			break;
		case ECollisionSide::Bot:
			CorrectedMove = Vector<float>(CorrectedMove.X, 0);
			break;
		}
	}

	VerifyVerticalCollision(CorrectedMove);
}

void Core::PhysicComponent::VerifyVerticalCollision(Vector<float>& CorrectedMove)
{
	float Tolerance = 3.f;

	if (mCollisionSides[0] != ECollisionSide::Undefined && mCollisionSides[1] != ECollisionSide::Undefined)
	{
		// 2 collisions enregistrées

		if (mCollisionSides[0] == ECollisionSide::Top || mCollisionSides[0] == ECollisionSide::Bot)
		{
			// Première collision == Top ou Bot

			if (mCollisionSides[1] == ECollisionSide::Right)
			{
				// Dexième collision == Right

				if (std::abs(mCollisionRects[1].X - mCollisionRects[0].X) <= Tolerance)
				{
					// allow Y move
					CorrectedMove.Y = mPendingMove.Y;
				}
			}
			else if (mCollisionSides[1] == ECollisionSide::Left)
			{
				// Dexième collision == Left

				if (std::abs((mCollisionRects[1].X + mCollisionRects[1].W) - (mCollisionRects[0].X + mCollisionRects[0].W)) <= Tolerance)
				{
					// allow Y move
					CorrectedMove.Y = mPendingMove.Y;
				}
			}
		}
		else
		{
			// Première collision == Left ou Right

			if (mCollisionSides[1] == ECollisionSide::Top)
			{
				// Dexième collision == Top 

				if (mCollisionSides[0] == ECollisionSide::Left)
				{
					// Première collision == Left

					if (std::abs((mCollisionRects[0].X + mCollisionRects[0].W) - (mCollisionRects[1].X + mCollisionRects[1].W)) <= Tolerance)
					{
						CorrectedMove.Y = mPendingMove.Y;
					}
				}
				else
				{
					// Première collision == Right

					if (std::abs(mCollisionRects[0].X - mCollisionRects[1].X) <= Tolerance)
					{
						CorrectedMove.Y = mPendingMove.Y;
					}
				}

			}
			else if (mCollisionSides[1] == ECollisionSide::Bot)
			{
				// Dexième collision == Bot

				if (mCollisionSides[0] == ECollisionSide::Left)
				{
					// Première collision == Left

					if (std::abs((mCollisionRects[0].X + mCollisionRects[0].W) - (mCollisionRects[1].X + mCollisionRects[1].W)) <= Tolerance)
					{
						CorrectedMove.Y = mPendingMove.Y;
					}
				}
				else
				{
					// Première collision == Right

					if (std::abs(mCollisionRects[0].X - mCollisionRects[1].X) <= Tolerance)
					{
						CorrectedMove.Y = mPendingMove.Y;
					}
				}
			}
		}

	}
}

void Core::PhysicComponent::Destroy()
{
}
