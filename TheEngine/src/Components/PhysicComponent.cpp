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

	// Si aucune collision, bouger le joueur � l'endroit de sa collision
	if (!bCollisionOccured)
	{
		Vector<float> OwnerCurrentLoc = mOwner->GetLocation();
		mOwner->SetLocation(OwnerCurrentLoc + mPendingMove);
	}
	else // sinon, remettre sa collision ou elle �tait avant
	{
		Vector<float> CorrectedMove = Vector<float>::ZeroVector();

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


		/////////////// test fix bug ///////////////////////////

		float Tolerance = 3.f;
		if (mCollisionSides[0] != ECollisionSide::Undefined && mCollisionSides[1] != ECollisionSide::Undefined)
		{
			// 2 collisions enregistr�es

			if (mCollisionSides[0] == ECollisionSide::Top || mCollisionSides[0] == ECollisionSide::Bot)
			{
				// Premi�re collision == Top ou Bot

				if (mCollisionSides[1] == ECollisionSide::Right)
				{
					// Dexi�me collision == Right

					if (std::abs(mCollisionRects[1].X - mCollisionRects[0].X) <= Tolerance)
					{
						// allow Y move
						CorrectedMove.Y = mPendingMove.Y;
					}
				}
				else if (mCollisionSides[1] == ECollisionSide::Left)
				{
					// Dexi�me collision == Left

					if (std::abs((mCollisionRects[1].X + mCollisionRects[1].W) - (mCollisionRects[0].X + mCollisionRects[0].W)) <= Tolerance)
					{
						// allow Y move
						CorrectedMove.Y = mPendingMove.Y;
					}
				}
			}
			else
			{
				// Premi�re collision == Left ou Right

				if (mCollisionSides[1] == ECollisionSide::Top)
				{
					// Dexi�me collision == Top 

					if (mCollisionSides[0] == ECollisionSide::Left)
					{
						// Premi�re collision == Left

						if (std::abs((mCollisionRects[0].X + mCollisionRects[0].W) - (mCollisionRects[1].X + mCollisionRects[1].W)) <= Tolerance)
						{
							CorrectedMove.Y = mPendingMove.Y;
						}
					}
					else
					{
						// Premi�re collision == Right

						if (std::abs(mCollisionRects[0].X - mCollisionRects[1].X) <= Tolerance)
						{
							CorrectedMove.Y = mPendingMove.Y;
						}
					}
					
				}
				else if (mCollisionSides[1] == ECollisionSide::Bot)
				{
					// Dexi�me collision == Bot

					if (mCollisionSides[0] == ECollisionSide::Left)
					{
						// Premi�re collision == Left

						if (std::abs((mCollisionRects[0].X + mCollisionRects[0].W) - (mCollisionRects[1].X + mCollisionRects[1].W)) <= Tolerance)
						{
							CorrectedMove.Y = mPendingMove.Y;
						}
					}
					else
					{
						// Premi�re collision == Right

						if (std::abs(mCollisionRects[0].X - mCollisionRects[1].X) <= Tolerance)
						{
							CorrectedMove.Y = mPendingMove.Y;
						}
					}
				}
			}
			
		}

		mOwner->SetLocation(mOwner->GetLocation() + CorrectedMove);
		//mCollisionComponent->SetCollisionLocation(mOwner->GetLocation());

		bCollisionOccured = false;
		mCollisionSides[0] = ECollisionSide::Undefined;
		mCollisionSides[1] = ECollisionSide::Undefined;
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
	//Vector<float> NewPos = OwnerPos + mVelocity * DeltaTime;
	mPendingMove = mVelocity * DeltaTime;
	
	if (mCollisionComponent)
	{
		if (mPendingMove != Vector<float>::ZeroVector())
		{
			// Add pending move to the collision to calculate collision
			mCollisionComponent->SetCollisionLocation(OwnerPos + mPendingMove);
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

		// movement dans deux axes
		if (mPendingMove.X != 0 && mPendingMove.Y != 0)
		{
			if (mCollisionSides[0] == ECollisionSide::Undefined)
			{
				// premiere collision d�tect�e cette frame-ci

				mCollisionRects[0] = OtherRect;

				float T = 10.f;

				if (OwnerRect.Y < (OtherRect.Y + OtherRect.H) && OwnerRect.Y + T > (OtherRect.Y + OtherRect.H) || OwnerRect.Y + OwnerRect.H > OtherRect.Y && OwnerRect.Y + OwnerRect.H - T < OtherRect.Y)
				{
					// g�rer top bot en premier

					if (mPendingMove.Y < 0 && Dir.Y < 0)
					{
						// top
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(CollisionLoc.X, OwnerLoc.Y));

						return ECollisionSide::Top;
					}
					else if (mPendingMove.Y > 0 && Dir.Y > 0)
					{
						// bot
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(CollisionLoc.X, OwnerLoc.Y));

						return ECollisionSide::Bot;
					}
				}
				else
				{
					// g�rer left right en premier

					if (mPendingMove.X > 0 && Dir.X > 0)
					{
						// droite
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(OwnerLoc.X, CollisionLoc.Y));

						return ECollisionSide::Right;
					}
					else if (mPendingMove.X < 0 && Dir.X < 0)
					{
						// gauche
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(OwnerLoc.X, CollisionLoc.Y));

						return ECollisionSide::Left;
					}
				}
						
			}
			else
			{
				// deuxieme collision d�tect�e cette frame-ci

				mCollisionRects[1] = OtherRect;

				if (mCollisionSides[0] == ECollisionSide::Left || mCollisionSides[0] == ECollisionSide::Right)
				{
					// collision gauche droite d�ja g�r�

					if (mPendingMove.Y < 0 && Dir.Y < 0)
					{
						// top
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(CollisionLoc.X, OwnerLoc.Y));

						return ECollisionSide::Top;
					}
					else if (mPendingMove.Y > 0 && Dir.Y > 0)
					{
						// bot
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(CollisionLoc.X, OwnerLoc.Y));

						return ECollisionSide::Bot;
					}
				}
				else if(mCollisionSides[0] == ECollisionSide::Top || mCollisionSides[0] == ECollisionSide::Bot)
				{
					// Collision top bot d�ja g�r�

					if (mPendingMove.X > 0 && Dir.X > 0)
					{
						// droite
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(OwnerLoc.X, CollisionLoc.Y));

						return ECollisionSide::Right;
					}
					else if (mPendingMove.X < 0 && Dir.X < 0)
					{
						// gauche
						Vector<float> OwnerLoc = mOwner->GetLocation();
						Vector<float> CollisionLoc = OwnerBox->GetCollisionLocation();
						OwnerBox->SetCollisionLocation(Vector<float>(OwnerLoc.X, CollisionLoc.Y));

						return ECollisionSide::Left;
					}
				}
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

		if (mPendingMove.X > 0 && Dir.X > 0)
		{
			// droite
			return ECollisionSide::Right;
		}
		else if (mPendingMove.X < 0 && Dir.X < 0)
		{
			// gauche
			return ECollisionSide::Left;
		}

		if (mPendingMove.Y < 0 && Dir.Y < 0)
		{
			// top
			return ECollisionSide::Top;
		}
		else if (mPendingMove.Y > 0 && Dir.Y > 0)
		{
			// bot
			return ECollisionSide::Bot;
		}

	}
	return ECollisionSide::Undefined;
}

void Core::PhysicComponent::Destroy()
{
}
