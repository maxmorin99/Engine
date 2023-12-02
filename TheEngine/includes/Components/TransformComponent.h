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
		virtual Component* Clone(Object* Owner) override;
		virtual void SetupClone(Component* Child) override;

	private:
		Vector<float> mLocation = Vector<float>::ZeroVector();
		Vector<float> mOldLocation = Vector<float>::ZeroVector();
		Vector<float> mSize = Vector<float>::ZeroVector();
		float mRotation = 0.f;

	public:
		/** Get the owner location */
		inline const Vector<float> GetLocation() const { return mLocation; }
		void GetLocation(float* OutX, float* OutY);
		inline const Vector<float> GetOldLocation() const { return mOldLocation; }
		inline void SetOldLocation(const Vector<float>& OldLoc) { mOldLocation = OldLoc; }
		/** Set the owner location */
		inline void SetLocation(const Vector<float>& NewLoc) { mLocation = NewLoc; }
		void SetLocation(float NewX, float NewY);

		/** Get the owner size */
		inline const Vector<float> GetSize() const { return mSize; }
		void GetSize(float* OutW, float* OutH) const;
		/** Set the owner size */
		inline void SetSize(const Vector<float>& NewSize) { mSize = NewSize; }
		void SetSize(float NewW, float NewH);

		/** Get the current rotation */
		inline float GetRotation() const { return mRotation; }
		/** Set the rotation */
		inline void SetRotation(float NewRot) { mRotation = NewRot; }

		/** Get the forward vector of the owner */
		Vector<float> GetForwardVector() const;
	};
}