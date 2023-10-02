#pragma once

#include "Component.h"
#include "Utility.h"

namespace Core
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(Object* Owner);
		TransformComponent(Object* Owner, const Vector<float>& Loc, const Vector<int>& Size, float Rotation);
		virtual ~TransformComponent() = default;
		virtual void Start();
		virtual void Destroy();

	private:
		Vector<float> mLocation = Vector<float>::ZeroVectorF;
		Vector<int> mSize = Vector<int>::ZeroVector;
		float mRotation = 0.f;

	public:
		inline const Vector<float> GetLocation() const { return mLocation; }
		void GetLocation(float* OutX, float* OutY);
		inline void SetLocation(const Vector<float>& NewLoc) { mLocation = NewLoc; }
		void SetLocation(float NewX, float NewY);

		inline const Vector<int> GetSize() const { return mSize; }
		void GetSize(int* OutW, int* OutH) const;
		inline void SetSize(const Vector<int>& NewSize) { mSize = NewSize; }
		void SetSize(int NewW, int NewH);

		inline float GetRotation() const { return mRotation; }
		inline void SetRotation(float NewRot) { mRotation = NewRot; }
	};
}