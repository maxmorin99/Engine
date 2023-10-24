#pragma once

#include "Utility.h"

namespace Core
{
	class RotationMatrix
	{
	public:
		void RotateRect(float Angle, const Rect<float>& ObjRect, Vector<float>* OutPoints);

	private:
		void CalculatePointsOfRect(const Rect<float>& InRect, Vector<float>* OutPoints);
		void Rotate(float Angle, Vector<float>* RectPoints, float X, float Y, float W, float H);
		void BuildRotationMatrix(float Matrix[3][3], float Angle);
		void BuildTranslationMatrices(float Matrix1[3][3], float Matrix2[3][3], float X, float Y, float W, float H);
		void BuildTransformationMatrix(float RotationMatrix[3][3], float TranslationMatrix1[3][3], float TranslationMatrix2[3][3], float TransformationMatrix[3][3]);
		void ApplyRotationToRectPoints(Vector<float>* Points, float Matrix[3][3]);
	};
}