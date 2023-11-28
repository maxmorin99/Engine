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
		Rect<float> mButtonRect;
		std::string mButtonText;
		Color mTextColor;
		Color mDefaultColor;
		Color mHoverColor;
		Color mCurrentColor;
		size_t mFontId;
		int mBorderSize;
		BoxComponent* mBoxComp;
		bool bHover = false;

	public:
		void SetBorderSize(int Size);
		void SetButtonRect(const Rect<float>& InRect);
		void SetFont(const std::string& File, int FontSize);
		void SetText(const std::string& Text);
		void SetDefaultColor(const Color& InColor);
		void SetTextColor(const Color& InColor);
		void SetHoverColor(const Color& InColor);
		void SetSizeRatio(const Vector<float>& Ratio);
		void SetPositionRatio(const Vector<float>& Ratio);

		inline Rect<float> GetButtonRect() const { return mButtonRect; }

	};
}