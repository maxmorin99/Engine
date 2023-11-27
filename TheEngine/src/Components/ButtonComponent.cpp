#include "Components/ButtonComponent.h"

Core::ButtonComponent::ButtonComponent(Object* Owner) :
	Component(Owner), mFontId(0), 
	mButtonText(""), mTextColor(Color::White), 
	mBorderSize(1), mButtonColor(Color::Black), 
	mScreenRatio(Vector<float>(0.1, 0.1))
{
}

void Core::ButtonComponent::SetFont(const std::string& File, int FontSize)
{
	mFontId = Graphic().LoadFont(File.c_str(), FontSize);
}

void Core::ButtonComponent::SetButtonColor(const Color& InColor)
{
	mButtonColor = InColor;
}

void Core::ButtonComponent::SetTextColor(const Color& InColor)
{
	mTextColor = InColor;
}

void Core::ButtonComponent::SetSizeRatio(const Vector<float>& Ratio)
{
	mScreenRatio = Ratio;


}

void Core::ButtonComponent::Draw()
{
	// Draw border
	for (int i = 0; i < mBorderSize; i++)
	{
		float NewX = mButtonRect.X + i;
		float NewY = mButtonRect.Y + i;
		float NewW = mButtonRect.W - i * 2;
		float NewH = mButtonRect.H - i * 2;
		Rect<float> NewRect{ NewX, NewY, NewW, NewH };
		Graphic().DrawRectF(false, &NewRect, Color::Black);
	}

	// Draw button
	Rect<float> FillRect{ mButtonRect.X + mBorderSize, mButtonRect.Y + mBorderSize, mButtonRect.W - mBorderSize * 2, mButtonRect.H - mBorderSize * 2 };
	Graphic().DrawRectF(true, &FillRect, mButtonColor);

	// Draw text
	Graphic().DrawString(mButtonText.c_str(), mFontId, mButtonRect.X, mButtonRect.Y, mButtonRect.W, mButtonRect.H, mTextColor);
}
