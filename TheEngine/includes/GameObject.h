#pragma once

#include "Utility.h"
#include "Object.h"

namespace Core
{
	class GameObject : public Object
	{
	public:
		GameObject() = default;
		virtual ~GameObject() = default;
		
		// Object interface
		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void Destroy() override;
		
	protected:
		Vector2D<float> mLocation = Vector2D<float>::ZeroVectorF;
		float mW = 0.f;
		float mH = 0.f;

	public:
		void GetObjectPosition(Vector2D<float>& outLocation) const;
		void GetObjectSize(float* OutW, float* OutH) const;
	};
}