#pragma once

#include "Component.h"
#include "Utility.h"

namespace Core
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(Object* Owner);
		TransformComponent(Object* Owner, const Vector<float>& Loc, const Vector<float>& Size, float Rotation);
		virtual ~TransformComponent() = default;
		virtual void Start();
		virtual void Destroy();

	private:
		Vector<float> mLocation = Vector<float>::ZeroVector();
		Vector<float> mOldLocation = Vector<float>::ZeroVector();
		Vector<float> mSize = Vector<float>::ZeroVector();
		float mRotation = 0.f;

	public:
		inline const Vector<float> GetLocation() const { return mLocation; }
		void GetLocation(float* OutX, float* OutY);
		inline const Vector<float> GetOldLocation() const { return mOldLocation; }
		inline void SetOldLocation(const Vector<float>& OldLoc) { mOldLocation = OldLoc; }
		inline void SetLocation(const Vector<float>& NewLoc) { mLocation = NewLoc; }
		void SetLocation(float NewX, float NewY);

		inline const Vector<float> GetSize() const { return mSize; }
		void GetSize(float* OutW, float* OutH) const;
		inline void SetSize(const Vector<float>& NewSize) { mSize = NewSize; }
		void SetSize(float NewW, float NewH);

		inline float GetRotation() const { return mRotation; }
		inline void SetRotation(float NewRot) { mRotation = NewRot; }

		Vector<float> GetForwardVector() const;
	};
}