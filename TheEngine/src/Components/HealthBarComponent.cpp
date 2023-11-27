#include "Components/HealthBarComponent.h"

Core::HealthBarComponent::HealthBarComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::HealthBarComponent::Start()
{
	Component::Start();

	int WindowW, WindowH = 0;
	Graphic().GetWindowSize(&WindowW, &WindowH);
	float HealthBarX = mPaddingPercent * WindowW;
	float HealthBarY = mPaddingPercent * WindowH;
	float HealthBarW = mSizeRatioToScreen.X * WindowW;
	float HealthBarH = mSizeRatioToScreen.Y * WindowH;
	SetBarRect(Rect<float>(HealthBarX, HealthBarY, HealthBarW, HealthBarH));
}

void Core::HealthBarComponent::Draw()
{
	Graphic().DrawRectF(false, &mBarRect, Color::Black);
	for (int i = 0; i < mBorderSize; i++)
	{
		float NewX = mBarRect.X + i;
		float NewY = mBarRect.Y + i;
		float NewW = mBarRect.W -  i * 2;
		float NewH = mBarRect.H - i * 2;
		Rect<float> NewRect{NewX, NewY, NewW, NewH};
		Graphic().DrawRectF(false, &NewRect, Color::Black);
	}

	Rect<float> FillRect{mBarRect.X + mBorderSize, mBarRect.Y + mBorderSize, 0.f, mBarRect.H - mBorderSize * 2};
	float FillRectW = (mBarRect.W * (mPercent / 100)) - (mBorderSize * 2);
	FillRect.W = FillRectW;
	Graphic().DrawRectF(true, &FillRect, mBarColor);
}

Core::Component* Core::HealthBarComponent::Clone(Object* Owner)
{
	HealthBarComponent* Clone = new HealthBarComponent(Owner);
	__super::SetupClone(Clone);

	return Clone;
}

void Core::HealthBarComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);
}
