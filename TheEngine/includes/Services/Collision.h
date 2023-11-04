#pragma once

#include "Utility.h"

namespace Core
{
	class Collision
	{
	public:
		Collision() = default;

		bool PointWithCircle(const Vector<float>& InPointLoc, const Vector<float>& InCircleLoc, const float InCircleRadius) const;
		bool CircleWithCircle(const Vector<float>& InCircleLoc_1, const float InCircleRadius_1, const Vector<float>& InCircleLoc_2, const float InCircleRadius_2) const;
		bool PointWithRect(const Vector<float>& InPointLoc, const Rect<float>& InRect) const;
		bool RectWithRect(const Rect<float>& InRect_1, const Rect<float>& InRect_2) const;
		bool RectWithCircle(const Rect<float>& InRect, const Vector<float>& InCircleLoc, const float InCircleRadius) const;
		bool PointWithLine(const Vector<float>& InPointLoc, const Vector<float>& LinePoint_1, const Vector<float>& LinePoint_2, const float Acceptance) const;

	private:
		Vector<float> GetNearestPointFromCircleAndRect(const Rect<float>& InRect, const Vector<float>& InCircleLoc) const;
	};
}