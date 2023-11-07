#pragma once

#include "Utility.h"

namespace Core
{
	class Collision
	{
	public:
		Collision() = default;

		static bool PointWithCircle(const Vector<float>& InPointLoc, const Vector<float>& InCircleLoc, const float InCircleRadius);
		static bool CircleWithCircle(const Vector<float>& InCircleLoc_1, const float InCircleRadius_1, const Vector<float>& InCircleLoc_2, const float InCircleRadius_2);
		static bool PointWithRect(const Vector<float>& InPointLoc, const Rect<float>& InRect);
		static bool RectWithRect(const Rect<float>& InRect_1, const Rect<float>& InRect_2, Vector<float>& OutCollisionPoint);
		static bool RectWithCircle(const Rect<float>& InRect, const Vector<float>& InCircleLoc, const float InCircleRadius);
		static bool PointWithLine(const Vector<float>& InPointLoc, const Vector<float>& LinePoint_1, const Vector<float>& LinePoint_2, const float Acceptance);

	private:
		static Vector<float> GetNearestPointFromCircleAndRect(const Rect<float>& InRect, const Vector<float>& InCircleLoc);
	};
}