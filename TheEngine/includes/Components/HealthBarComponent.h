#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IObserver.h"
#include <unordered_map>
#include <string>

namespace Core
{
	class HealthBarComponent : public Component, public IDrawable, public IObserver<std::unordered_map<std::string, void*>>
	{
	public:
		HealthBarComponent(Object* Owner);
		virtual ~HealthBarComponent() = default;
		virtual void Start() override;
		virtual void Draw() override;
		virtual Component* Clone(Object* Owner);
		virtual void SetupClone(Component* Child);

	private:
		Rect<float> mBarRect = Rect<float>(0, 0, 0, 0);
		Color mBarColor = Color::Red;
		float mPercent = 100.f;
		int mBorderSize = 12;
		/** % of the screen size to offset the top left corner of the health bar from the screen's top left corner */
		float mPaddingPercent = 0.f;
		/** % of the screen size that represents the health bar dimensions */
		Vector<float> mSizeRatioToScreen = Vector<float>(0.5f, 0.5f);

	public:
		/** Set the rect that defines
		*	@param InRect: Rect to use for the bar's rect
		*/
		inline void SetBarRect(const Rect<float>& InRect) { mBarRect = InRect; }

		/** Set the fill color of the health bar.
		*	@param InColor: Color to use for the bar
		*/
		inline void SetColor(const Color& InColor) { mBarColor = InColor; }

		/** Set the percentage of the health bar
		*	@param InPercent: Percent to use
		*/
		inline void SetPercent(float InPercent) { mPercent = InPercent; }

		/** Set the bar's border size in pixels
		*	@param Size: Pixels count to use for the border size
		*/
		inline void SetBorderSize(int Size) { mBorderSize = Size; }

		/** Value should be >=0 or <= 1
		*	@param Percent: Percentage to use for the bar padding
		*/
		inline void SetPaddingPercent(float Percent) { mPaddingPercent = Percent; }

		/** Set the % of the screen size that represents the bar size. Value should be >=0 or <= 1
		*	@param Ratio: Ratio to use
		*/
		void SetSizeRatio(const Vector<float>& Ratio) { mSizeRatioToScreen = Ratio; }

		/** Called when the player take damage */
		virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;
	};
}