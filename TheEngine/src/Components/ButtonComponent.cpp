#include "Components/ButtonComponent.h"
#include "Components/BoxComponent.h"
#include "Object.h"

Core::ButtonComponent::ButtonComponent(Object* Owner) :
	Component(Owner), mFontId(0), 
	mButtonText(""), mTextColor(Color::White), 
	mBorderSize(1), mDefaultColor(Color::Black),
	mBoxComp(nullptr), mHoverColor(Color::Grey),
	mCurrentColor(mDefaultColor)
{
	if (mOwner)
	{
		mBoxComp = mOwner->AddComponent<BoxComponent>();
		mBoxComp->SetCollisionChannel(ECollisionChannel::UI);
		mBoxComp->SetCollisionResponseToChannel(ECollisionChannel::UI, ECollisionResponse::Overlap);
		mBoxComp->BindOnCollisionOverlapBegin(this);
		mBoxComp->BindOnCollisionOverlapEnd(this);
		mBoxComp->SetIsMovableCollision(false);
	}
}

void Core::ButtonComponent::SetBorderSize(int Size)
{
	mBorderSize = Size;
}

void Core::ButtonComponent::SetButtonRect(const Rect<float>& InRect)
{
	mButtonRect = InRect;
	if (mBoxComp)
	{
		mBoxComp->SetCollisionLocation(Vector<float>(InRect.X, InRect.Y));
		mBoxComp->SetBoxSize(InRect.W, InRect.H);
	}
}

void Core::ButtonComponent::SetFont(const std::string& File, int FontSize)
{
	mFontId = Graphic().LoadFont(File.c_str(), FontSize);
}

void Core::ButtonComponent::SetText(const std::string& Text)
{
	mButtonText = Text;
}

void Core::ButtonComponent::SetDefaultColor(const Color& InColor)
{
	mDefaultColor = InColor;
	mCurrentColor = InColor;
}

void Core::ButtonComponent::SetTextColor(const Color& InColor)
{
	mTextColor = InColor;
}

void Core::ButtonComponent::SetHoverColor(const Color& InColor)
{
	mHoverColor = InColor;
}

void Core::ButtonComponent::SetSizeRatio(const Vector<float>& Ratio)
{
	int ScreenW, ScreenH = 0;
	Graphic().GetWindowSize(&ScreenW, &ScreenH);
	mButtonRect.W = Ratio.X * ScreenW;
	mButtonRect.H = Ratio.Y * ScreenH;
	if (mBoxComp)
	{
		mBoxComp->SetBoxSize(mButtonRect.W, mButtonRect.H);
	}
}

void Core::ButtonComponent::SetPositionRatio(const Vector<float>& Ratio)
{
	int ScreenW, ScreenH = 0;
	Graphic().GetWindowSize(&ScreenW, &ScreenH);
	mButtonRect.X = Ratio.X * ScreenW;
	mButtonRect.Y = Ratio.Y * ScreenH;
	if (mBoxComp)
	{
		mBoxComp->SetCollisionLocation(Vector<float>(mButtonRect.X, mButtonRect.Y));
	}
}

void Core::ButtonComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	// récupérer le bool "bBeginOverlap" et le bool "bEndOverlap"
	void* bBeginVoidPtr = nullptr;
	void* OtherObjectVoidPtr = nullptr;
	void* OtherComponentVoidPtr = nullptr;

	if (Value.count("bBeginOverlap"))
	{
		bBeginVoidPtr = Value.at("bBeginOverlap");
	}
	if (Value.count("OtherObject"))
	{
		OtherObjectVoidPtr = Value.at("OtherObject");
	}
	if (Value.count("OtherComponent"))
	{
		OtherComponentVoidPtr = Value.at("OtherComponent");
	}

	bool* bBegin = static_cast<bool*>(bBeginVoidPtr) ? static_cast<bool*>(bBeginVoidPtr) : nullptr;
	Object* OtherObject = static_cast<Object*>(OtherObjectVoidPtr) ? static_cast<Object*>(OtherObjectVoidPtr) : nullptr;
	Component* OtherComponent = static_cast<Component*>(OtherComponentVoidPtr) ? static_cast<Component*>(OtherComponentVoidPtr) : nullptr;

	if (bBegin && OtherObject)
	{
		if (OtherObject->HasTag("Cursor"))
		{
			if (*bBegin)
			{
				// change color
				mCurrentColor = mHoverColor;
			}
			else
			{
				// change color
				mCurrentColor = mDefaultColor;
			}
		}
	}
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
	Graphic().DrawRectF(true, &FillRect, mCurrentColor);

	// Draw text
	Graphic().DrawString(mButtonText.c_str(), mFontId, FillRect.X, FillRect.Y, FillRect.W, FillRect.H, mTextColor);
}
