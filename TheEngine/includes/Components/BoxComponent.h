#pragma once

#include "CollisionComponent.h"
#include "Utility.h"
#include "Interfaces/IUpdatable.h"
#include "Interfaces/IDrawable.h"

namespace Core
{
	class BoxComponent : public CollisionComponent, public IUpdatable, public IDrawable
	{
	public:
		BoxComponent(Object* Owner);
		virtual void Update(float DeltaTime) override;
		virtual void Draw() override;

	private:
		Rect<float> mRect;
		Vector<float> mOwnerOffset = Vector<float>::ZeroVector();

	public:
		void SetOffset(float OffsetX, float OffsetY);
		void SetBoxSize(float SizeX, float SizeY);
	};
}