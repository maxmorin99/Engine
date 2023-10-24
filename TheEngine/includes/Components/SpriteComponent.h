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

	protected:
		std::string mFile = "";
		size_t mTextureId = 0;
		Rect<int> mSrc = Rect<int>(0, 0, 0, 0);
		Rect<int> mDst = Rect<int>(0, 0, 0, 0);
		Color mColor = Color::White;
		Flip mFlip = Flip::None;

	public:
		inline void SetColor(const Color& NewColor) { mColor = NewColor; }
		inline const Color& getColor() const { return mColor; }
		void SetFile(const std::string& File);
		void SetFlip(const Flip& InFlip);
		inline Flip GetFlip() const { return mFlip; }
	};
}