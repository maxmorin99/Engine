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
		SpriteComponent(Object* Owner, const TextureData& Data);
		virtual ~SpriteComponent() = default;
		virtual void Start();
		virtual void Destroy();

		virtual void Draw() override;

	protected:
		TextureData mTextureData;
		size_t mTextureId = 0;
		Vector<int> mTextureSize = Vector<int>::ZeroVector();
		Rect<int> mSrc = Rect<int>(0.f, 0.f, 0.f, 0.f);

		/** Color of the sprite. */
		Color mColor = Color::White;

	public:
		inline void SetColor(const Color& NewColor) { mColor = NewColor; }
		inline const Color& getColor() const { return mColor; }

		inline void SetTextureData(const TextureData& Data) { mTextureData = Data; }
	};
}