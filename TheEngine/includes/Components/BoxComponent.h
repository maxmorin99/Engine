#pragma once

#include "CollisionComponent.h"
#include "Utility.h"
#include "Interfaces/IUpdatable.h"
#include "Interfaces/IDrawable.h"

namespace Core
{
	class BoxComponent : public CollisionComponent, public IDrawable
	{
	public:
		BoxComponent(Object* Owner);
		virtual void Start() override;
		virtual void Update(float DeltaTime) override;
		virtual void Draw() override;
		virtual Component* Clone(Object* Owner) override;
		virtual void SetupClone(Component* Child) override;
		virtual void SetCollisionLocation(const Vector<float>& NewLoc);
		virtual Vector<float> GetCollisionLocation() const override;
		virtual void OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp) override;

	private:
		/** Rect that defines the collision boundaries */
		Rect<float> mRect;

		/** Vector value to add to the rect location to fit the owner sprite(s) */
		Vector<float> mOwnerOffset = Vector<float>::ZeroVector();
		bool bMovableCollision = true;

	public:
		/** Set the Vector value to add to the rect location to fit the owner sprite(s)
		*	@param OffsetX: X value to offset the component from its owner
		*	@param OffsetY: Y value to offset the component from its owner
		*/
		void SetOffset(float OffsetX, float OffsetY);

		/** Get the Vector value to add to the rect location to fit the owner sprite(s) */
		inline Vector<float> GetOffset() const { return mOwnerOffset; }

		/** Set the collision box dimensions
		*	@param SizeX: X dimension
		*	@param SizeY: Y dimension
		*/
		void SetBoxSize(float SizeX, float SizeY);

		/** Get the rect that defines the box boundaries of this collision */
		inline Rect<float> GetRect() const { return mRect; }

		/** Set whether this is a movable collision, which means that its owner can move
		*	@param State: Movable or not
		*/
		inline void SetIsMovableCollision(bool State) { bMovableCollision = State; }
	};
}