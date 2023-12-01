#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IUpdatable.h"
#include "Interfaces/IObserver.h"
#include "Subject.h"
#include <string>

namespace Core
{
	class BoxComponent;

	class ButtonComponent : public Component, public IDrawable, public IUpdatable, public IObserver<std::unordered_map<std::string, void*>>
	{
	public:
		ButtonComponent(Object* Owner);
		virtual ~ButtonComponent() = default;
		void Update(float DeltaTime) override;
		virtual void Draw() override;
		virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

		Subject<std::unordered_map<std::string, void*>> mOnClickSubject;

	private:
		/** Rect that defines the button's boundaries */
		Rect<float> mButtonRect;

		std::string mButtonText;

		Color mTextColor;

		/** Default button color */
		Color mDefaultColor;

		/** Color for when the button is hovered by the cursor */
		Color mHoverColor;

		Color mCurrentColor;
		size_t mFontId;

		/** Size in pixels of the button's borders */
		int mBorderSize;

		/** Reference to the owner's collision */
		BoxComponent* mBoxComp;

		/** Is the button hovered by the cursor? */
		bool bHover = false;

	public:
		/** Set the button's border size, in pixels */
		void SetBorderSize(int Size);

		/** Set the rect that defines the button's boundaries */
		void SetButtonRect(const Rect<float>& InRect);

		/** Set the font for the button's text */
		void SetFont(const std::string& File, int FontSize);

		/** Set the button's text */
		void SetText(const std::string& Text);

		/** Set the button default color */
		void SetDefaultColor(const Color& InColor);

		/** Set the button's text color */
		void SetTextColor(const Color& InColor);

		/** Set the button color when the cursor is on it */
		void SetHoverColor(const Color& InColor);

		/** Set the button size scale ratio compared to the screen size */
		void SetSizeRatio(const Vector<float>& Ratio);

		/** Set the button location scale ratio compared to the screen size */
		void SetPositionRatio(const Vector<float>& Ratio);

		/** Get the rect that defines the button boundaries */
		inline Rect<float> GetButtonRect() const { return mButtonRect; }
	};
}