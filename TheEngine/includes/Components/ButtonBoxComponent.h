#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include <unordered_map>

namespace Core
{
	class ButtonComponent;

	class ButtonBoxComponent : public Component, public IDrawable
	{
	public:
		ButtonBoxComponent(Object* Owner);
		virtual ~ButtonBoxComponent() = default;
		void AddButton(ButtonComponent* Button, float SizePercent);
		
		virtual void Draw() override;

	private:
		Rect<float> mRect;
		/** Key -> Btn, Key -> Item Vertical size */
		std::unordered_map<ButtonComponent*, float> mItemMap;
		float mPadding = 1.f;

		float GetFirstAvailablePositionY();

	public:
		inline void SetRect(const Rect<float>& InRect) { mRect = InRect; }
		inline void SetPadding(float InPadding) { mPadding = InPadding; }
	};
}