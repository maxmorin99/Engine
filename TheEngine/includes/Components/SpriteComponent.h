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
		SpriteComponent(Object* Owner, const std::string& File);
		virtual ~SpriteComponent() = default;
		virtual void Start();
		virtual void Destroy();

		virtual void Draw() override;

	private:
		/** Path of the texture */
		std::string mFile;

		size_t mTextureId = 0;

		/** Color of the sprite. */
		Color mColor = Color::White;

	public:
		inline void SetFile(const std::string& File) { mFile = File; }
		inline const std::string& GetFile() const { return mFile; }

		inline void SetColor(const Color& NewColor) { mColor = NewColor; }
		inline const Color& getColor() const { return mColor; }
	};
}