#include "Components/ButtonBoxComponent.h"
#include "Components/ButtonComponent.h"

Core::ButtonBoxComponent::ButtonBoxComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::ButtonBoxComponent::AddButton(ButtonComponent* Button, float SizePercent)
{
	if (!Button || mItemMap.count(Button) > 0) return;

	float TotalPercent = 0.f;
	for (auto& pair : mItemMap)
	{
		TotalPercent += pair.second;
	}

	// Box is full
	if (TotalPercent == 1)
	{
		return;
	}
	else if (TotalPercent + SizePercent > 1)
	{
		// resize the new button to the remaining space
		float RemainingSpace = 1 - TotalPercent;
		float ButtonX = mRect.X;
		float ButtonW = mRect.W;
		float ButtonY = GetFirstAvailablePositionY() == 0.f ? mRect.Y + mPadding : GetFirstAvailablePositionY() + mPadding;
		float ButtonH = mRect.H * RemainingSpace;
		Button->SetButtonRect(Rect<float>(ButtonX, ButtonY, ButtonW, ButtonH));
		mItemMap[Button] = RemainingSpace;
	}
	else
	{
		float ButtonX = mRect.X;
		float ButtonW = mRect.W;
		float ButtonY = GetFirstAvailablePositionY() == 0.f ? mRect.Y + mPadding : GetFirstAvailablePositionY() + mPadding;
		float ButtonH = mRect.H * SizePercent - mPadding;

		Button->SetButtonRect(Rect<float>(ButtonX, ButtonY, ButtonW, ButtonH));
		mItemMap[Button] = SizePercent;
	}
}

float Core::ButtonBoxComponent::GetFirstAvailablePositionY()
{
	float FirstY = 0.f;
	for (auto& pair : mItemMap)
	{
		FirstY = 0.f;
		float ItemEndY = pair.first->GetButtonRect().Y + pair.first->GetButtonRect().H;
		FirstY += ItemEndY;
	}
	return FirstY;
}

void Core::ButtonBoxComponent::Draw()
{
	//Graphic().DrawRectF(false, &mRect, Color::Red);
}
