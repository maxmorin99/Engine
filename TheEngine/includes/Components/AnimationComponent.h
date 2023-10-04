#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IUpdatable.h"

namespace Core
{
	class AnimationComponent : public Component, public IDrawable, public IUpdatable
	{
	public:
		AnimationComponent(Object* Owner);
		virtual ~AnimationComponent() = default;
		virtual void Start();
		virtual void Destroy();

		virtual void Draw() override;

	protected:
		TextureData mTextureData;
		size_t mTextureId = 0;
		Vector<int> mTextureSize = Vector<int>::ZeroVector();
		Rect<int> mSrc = Rect<int>(0, 0, 0, 0);
		Rect<int> mDst = Rect<int>(0, 0, 0, 0);
		Color mColor = Color::White;

	public:
		inline void SetColor(const Color& NewColor) { mColor = NewColor; }
		inline const Color& getColor() const { return mColor; }

		inline void SetTextureData(const TextureData& Data) { mTextureData = Data; }
	};
}