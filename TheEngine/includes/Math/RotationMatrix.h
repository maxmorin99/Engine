#pragma once

#include "Utility.h"

namespace Core
{
	/**
	* @brief A class for rotating rectangles using transformation matrices.
	*/
	class RotationMatrix
	{
	public:
		/**
		* @brief Rotates the given rectangle and returns the rotated points.
		*
		* @param Angle The rotation angle in radians.
		* @param ObjRect The rectangle to be rotated.
		* @param OutPoints Pointer to the vector to store the rotated points.
		*/
		void RotateRect(float Angle, const Rect<float>& ObjRect, Vector<float>* OutPoints);

	private:
		/**
		* @brief Calculates the points of the rectangle.
		*
		* @param InRect The input rectangle.
		* @param OutPoints Pointer to the vector to store the calculated points.
		*/
		void CalculatePointsOfRect(const Rect<float>& InRect, Vector<float>* OutPoints);

		/**
		* @brief Applies rotation to the rectangle points.
		*
		* @param Angle The rotation angle in radians.
		* @param RectPoints Pointer to the rectangle points.
		* @param X The X-coordinate of the rectangle.
		* @param Y The Y-coordinate of the rectangle.
		* @param W The width of the rectangle.
		* @param H The height of the rectangle.
		*/
		void Rotate(float Angle, Vector<float>* RectPoints, float X, float Y, float W, float H);

		/**
		* @brief Builds a 3x3 rotation matrix for the given angle.
		*
		* @param Matrix The rotation matrix to be built.
		* @param Angle The rotation angle in radians.
		*/
		void BuildRotationMatrix(float Matrix[3][3], float Angle);

		/**
		* @brief Builds translation matrices for the given parameters.
		*
		* @param Matrix1 The first translation matrix to be built.
		* @param Matrix2 The second translation matrix to be built.
		* @param X The X-coordinate of the rectangle.
		* @param Y The Y-coordinate of the rectangle.
		* @param W The width of the rectangle.
		* @param H The height of the rectangle.
		*/
		void BuildTranslationMatrices(float Matrix1[3][3], float Matrix2[3][3], float X, float Y, float W, float H);

		/**
		* @brief Builds a 3x3 transformation matrix from rotation and translation matrices.
		*
		* @param RotationMatrix The rotation matrix.
		* @param TranslationMatrix1 The first translation matrix.
		* @param TranslationMatrix2 The second translation matrix.
		* @param TransformationMatrix The resulting transformation matrix.
		*/
		void BuildTransformationMatrix(float RotationMatrix[3][3], float TranslationMatrix1[3][3], float TranslationMatrix2[3][3], float TransformationMatrix[3][3]);

		/**
		* @brief Applies the rotation matrix to the rectangle points.
		*
		* @param Points Pointer to the rectangle points.
		* @param Matrix The rotation matrix.
		*/
		void ApplyRotationToRectPoints(Vector<float>* Points, float Matrix[3][3]);
	};
}