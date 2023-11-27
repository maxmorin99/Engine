#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include <string>

namespace Core
{
	class ButtonComponent : public Component, public IDrawable
	{
	public:
		ButtonComponent(Object* Owner);
		virtual ~ButtonComponent() = default;
		virtual void Draw() override;

	private:
		Rect<float> mButtonRect;
		std::string mButtonText;
		Color mTextColor;
		Color mButtonColor;
		size_t mFontId;
		int mBorderSize;
		Vector<float> mScreenRatio;

	public:
		void SetFont(const std::string& File, int FontSize);
		void SetButtonColor(const Color& InColor);
		void SetTextColor(const Color& InColor);
		void SetSizeRatio(const Vector<float>& Ratio);

		
	};
}