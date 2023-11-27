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
		Rect<float> mRect;
		Vector<float> mOwnerOffset = Vector<float>::ZeroVector();

		

	public:
		void SetOffset(float OffsetX, float OffsetY);
		void SetBoxSize(float SizeX, float SizeY);
		inline Rect<float> GetRect() const { return mRect; }
	};
}