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
		/** Key -> Btn, Value -> Vertical size */
		std::unordered_map<ButtonComponent*, float> mItemMap;

		/** Padding between buttons in this box */
		float mPadding = 1.f;

		/** Returns the first Y coordinates available to place another button */
		float GetFirstAvailablePositionY();

	public:
		/** Set the rect that defines the box boundaries
		*	@param InRect: The rect that will be used to set the button's rect
		*/
		inline void SetRect(const Rect<float>& InRect) { mRect = InRect; }

		/** Set the padding value to apply between eah buttons
		*	@param InPadding: Value that represents the padding to apply
		*/
		inline void SetPadding(float InPadding) { mPadding = InPadding; }
	};
}