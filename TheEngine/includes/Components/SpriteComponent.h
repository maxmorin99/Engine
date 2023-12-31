#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include <string>
#include "Utility.h"

namespace Core
{
	class SpriteComponent : public Component, public IDrawable
	{
	public:
		SpriteComponent(Object* Owner);
		virtual ~SpriteComponent() = default;
		virtual void Start();
		virtual void Destroy();
		virtual void Draw() override;
		virtual Component* Clone(Object* Owner) override;
		virtual void SetupClone(Component* Child) override;

	protected:
		std::string mFile = "";
		size_t mTextureId = 0;
		Rect<int> mSrc = Rect<int>(0, 0, 0, 0);
		Rect<int> mDst = Rect<int>(0, 0, 0, 0);
		Color mColor = Color::White;
		Flip mFlip = Flip::None;

		/** Offset to add to the sprite's destination rect to get to the center of the image */
		Vector<float> mCenterOffset = Vector<float>::ZeroVector();

	public:
		/** Set the image color */
		inline void SetColor(const Color& NewColor) { mColor = NewColor; }

		/** Get the image color */
		inline const Color& getColor() const { return mColor; }

		/** Set the source image path */
		void SetFile(const std::string& File);
		void SetFile(const std::string& File, int IdxX, int IdxY, int ImgCountW, int ImgCountH);

		/** Set the image flip */
		void SetFlip(const Flip& InFlip);

		/** Get the image flip */
		inline Flip GetFlip() const { return mFlip; }
		inline void SetCenterOffset(const Vector<float>& CenterOffset) { mCenterOffset = CenterOffset; }
		inline Vector<float> GetCenterOffset() const { return mCenterOffset; }
	};
}