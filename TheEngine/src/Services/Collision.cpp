#include "Services/Collision.h"

bool Core::Collision::PointWithCircle(const Vector<float>& InPointLoc, const Vector<float>& InCircleLoc, const float InCircleRadius)
{
	float DistanceTotal = Vector<float>::Dist(InPointLoc, InCircleLoc);
	return DistanceTotal < InCircleRadius;
}

bool Core::Collision::CircleWithCircle(const Vector<float>& InCircleLoc_1, const float InCircleRadius_1, const Vector<float>& InCircleLoc_2, const float InCircleRadius_2)
{
	float DistanceTotal = Vector<float>::Dist(InCircleLoc_1, InCircleLoc_2);
	float RadiusSum = InCircleRadius_1 + InCircleRadius_2;
	return DistanceTotal < RadiusSum;
}

bool Core::Collision::PointWithRect(const Vector<float>& InPointLoc, const Rect<float>& InRect)
{
	if (InPointLoc.X < InRect.X) return false;
	if (InPointLoc.Y < InRect.Y) return false;
	if (InPointLoc.X > InRect.X + InRect.W) return false;
	if (InPointLoc.Y > InRect.Y + InRect.H) return false;
	return true;
}

bool Core::Collision::RectWithRect(const Rect<float>& InRect_1, const Rect<float>& InRect_2, Vector<float>& OutCollisionPoint)
{
	OutCollisionPoint = Vector<float>::ZeroVector();

	if (InRect_1.X > InRect_2.X + InRect_2.W) return false;
	if (InRect_1.X + InRect_1.W < InRect_2.X) return false;
	if (InRect_1.Y > InRect_2.Y + InRect_2.H) return false;
	if (InRect_1.Y + InRect_1.H < InRect_2.Y) return false;

	float Left = std::max(InRect_1.X, InRect_2.X);
	float Top = std::max(InRect_1.Y, InRect_2.Y);
	float Right = std::min(InRect_1.X + InRect_1.W, InRect_2.X + InRect_2.W);
	float Bottom = std::min(InRect_1.Y + InRect_1.H, InRect_2.Y + InRect_2.H);

	float W = abs(Left - Right);
	float H = abs(Top - Bottom);

	OutCollisionPoint = Vector<float>(Left + W / 2, Top + H / 2);
	return true;
}

bool Core::Collision::RectWithCircle(const Rect<float>& InRect, const Vector<float>& InCircleLoc, const float InCircleRadius)
{
	if (InCircleLoc.X >= InRect.X && InCircleLoc.X <= InRect.X + InRect.W)
	{
		if (InCircleLoc.Y >= InRect.Y && InCircleLoc.Y <= InRect.Y + InRect.H) return true;
	}
	Vector<float> NearestPoint = GetNearestPointFromCircleAndRect(InRect, InCircleLoc);
	return PointWithCircle(NearestPoint, InCircleLoc, InCircleRadius);
}

Core::Vector<float> Core::Collision::GetNearestPointFromCircleAndRect(const Rect<float>& InRect, const Vector<float>& InCircleLoc)
{
	Vector<float> NearestPoint = InCircleLoc;
	NearestPoint.X = NearestPoint.X < InRect.X ? InRect.X : InRect.X + InRect.W;
	NearestPoint.Y = NearestPoint.Y < InRect.Y ? InRect.Y : InRect.Y + InRect.H;
	return NearestPoint;
}

bool Core::Collision::PointWithLine(const Vector<float>& InPointLoc, const Vector<float>& LinePoint_1, const Vector<float>& LinePoint_2, const float Acceptance)
{
	float LineDistance = Vector<float>::Dist(LinePoint_1, LinePoint_2);
	float HalfDistanceFromPoint1 = Vector<float>::Dist(LinePoint_1, InPointLoc);
	float HalfDistanceFromPoint2 = Vector<float>::Dist(LinePoint_2, InPointLoc);
	float DistanceFromPoint = HalfDistanceFromPoint1 + HalfDistanceFromPoint2;
	return abs(DistanceFromPoint - LineDistance) <= Acceptance;
}
