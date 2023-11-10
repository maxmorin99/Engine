#include "Math/RotationMatrix.h"

/*
* Formules tiré de mon livre de mathématique
*/

void Core::RotationMatrix::RotateRect(float Angle, const Rect<float>& ObjRect, Vector<float>* OutPoints)
{
	Vector<float> RectPoints[4] = { {ObjRect.X, ObjRect.Y}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0} };
	CalculatePointsOfRect(ObjRect, RectPoints);
	Rotate(Angle, RectPoints, ObjRect.X, ObjRect.Y, ObjRect.W, ObjRect.H);

	OutPoints[0] = RectPoints[0];
	OutPoints[1] = RectPoints[1];
	OutPoints[2] = RectPoints[2];
	OutPoints[3] = RectPoints[3];
}

void Core::RotationMatrix::CalculatePointsOfRect(const Rect<float>& InRect, Vector<float>* OutPoints)
{
	OutPoints[0].X = InRect.X;
	OutPoints[0].Y = InRect.Y;
	OutPoints[1].X = InRect.X + InRect.W;
	OutPoints[1].Y = InRect.Y;
	OutPoints[2].X = InRect.X;
	OutPoints[2].Y = InRect.Y + InRect.H;
	OutPoints[3].X = InRect.X + InRect.W;
	OutPoints[3].Y = InRect.Y + InRect.H;
}

void Core::RotationMatrix::Rotate(float Angle, Vector<float>* RectPoints, float X, float Y, float W, float H)
{
	// Initialize empty matrices
	float RotationMatrix[3][3] = { 0 };
	float TransMatrix1[3][3] = { 0 };
	float TransMatrix2[3][3] = { 0 };
	float TransformationMatrix[3][3] = { 0 };
	// convert deg angle to rad
	Angle = (float)(Angle * PI / 180);

	// Build matrices
	BuildRotationMatrix(RotationMatrix, Angle);
	BuildTranslationMatrices(TransMatrix1, TransMatrix2, X, Y, W, H);
	BuildTransformationMatrix(RotationMatrix, TransMatrix1, TransMatrix2, TransformationMatrix);

	// Apply the rotation to the 4 points of the rect
	ApplyRotationToRectPoints(RectPoints, TransformationMatrix);
}

void Core::RotationMatrix::BuildRotationMatrix(float Matrix[3][3], float Angle)
{
	Matrix[0][0] = cos(Angle);
	Matrix[0][1] = -(sin(Angle));
	Matrix[0][2] = 0;
	Matrix[1][0] = sin(Angle);
	Matrix[1][1] = cos(Angle);
	Matrix[1][2] = 0;
	Matrix[2][0] = 0;
	Matrix[2][1] = 0;
	Matrix[2][2] = 1;
}

void Core::RotationMatrix::BuildTranslationMatrices(float Matrix1[3][3], float Matrix2[3][3], float X, float Y, float W, float H)
{
	// Ramener le centre du rect à l'origine 0,0
	Matrix1[0][0] = Matrix1[1][1] = Matrix1[2][2] = 1;
	Matrix1[0][1] = Matrix1[1][0] = Matrix1[2][0] = Matrix1[2][1] = 0;
	Matrix1[0][2] = -(X + (W / 2));
	Matrix1[1][2] = -(Y + (H / 2));

	// Ramener le rect à sa position initiale
	Matrix2[0][0] = Matrix2[1][1] = Matrix2[2][2] = 1;
	Matrix2[0][1] = Matrix2[1][0] = Matrix2[2][0] = Matrix2[2][1] = 0;
	Matrix2[0][2] = X + (W / 2);
	Matrix2[1][2] = Y + (H / 2);
}

void Core::RotationMatrix::BuildTransformationMatrix(float RotationMatrix[3][3], float TranslationMatrix1[3][3], float TranslationMatrix2[3][3], float TransformationMatrix[3][3])
{
	//tr = t3(T2) * t2(R) * t1(T1)

	TransformationMatrix[0][0] = (TranslationMatrix2[0][0] * RotationMatrix[0][0]) + (TranslationMatrix2[0][1] * RotationMatrix[1][0]) + (TranslationMatrix2[0][2] * RotationMatrix[2][0]);
	TransformationMatrix[1][0] = (TranslationMatrix2[1][0] * RotationMatrix[0][0]) + (TranslationMatrix2[1][1] * RotationMatrix[1][0]) + (TranslationMatrix2[1][2] * RotationMatrix[2][0]);
	TransformationMatrix[2][0] = (TranslationMatrix2[2][0] * RotationMatrix[0][0]) + (TranslationMatrix2[2][1] * RotationMatrix[1][0]) + (TranslationMatrix2[2][2] * RotationMatrix[2][0]);
															 													 													 
	TransformationMatrix[0][1] = (TranslationMatrix2[0][0] * RotationMatrix[0][1]) + (TranslationMatrix2[0][1] * RotationMatrix[1][1]) + (TranslationMatrix2[0][2] * RotationMatrix[2][1]);
	TransformationMatrix[1][1] = (TranslationMatrix2[1][0] * RotationMatrix[0][1]) + (TranslationMatrix2[1][1] * RotationMatrix[1][1]) + (TranslationMatrix2[1][2] * RotationMatrix[2][1]);
	TransformationMatrix[2][1] = (TranslationMatrix2[2][0] * RotationMatrix[0][1]) + (TranslationMatrix2[2][1] * RotationMatrix[1][1]) + (TranslationMatrix2[2][2] * RotationMatrix[2][1]);
																												 													 
	TransformationMatrix[0][2] = (TranslationMatrix2[0][0] * RotationMatrix[0][2]) + (TranslationMatrix2[0][1] * RotationMatrix[1][2]) + (TranslationMatrix2[0][2] * RotationMatrix[2][2]);
	TransformationMatrix[1][2] = (TranslationMatrix2[1][0] * RotationMatrix[0][2]) + (TranslationMatrix2[1][1] * RotationMatrix[1][2]) + (TranslationMatrix2[1][2] * RotationMatrix[2][2]);
	TransformationMatrix[2][2] = (TranslationMatrix2[2][0] * RotationMatrix[0][2]) + (TranslationMatrix2[2][1] * RotationMatrix[1][2]) + (TranslationMatrix2[2][2] * RotationMatrix[2][2]);


	TransformationMatrix[0][0] = (TransformationMatrix[0][0] * TranslationMatrix1[0][0]) + (TransformationMatrix[0][1] * TranslationMatrix1[1][0]) + (TransformationMatrix[0][2] * TranslationMatrix1[2][0]);
	TransformationMatrix[1][0] = (TransformationMatrix[1][0] * TranslationMatrix1[0][0]) + (TransformationMatrix[1][1] * TranslationMatrix1[1][0]) + (TransformationMatrix[1][2] * TranslationMatrix1[2][0]);
	TransformationMatrix[2][0] = (TransformationMatrix[2][0] * TranslationMatrix1[0][0]) + (TransformationMatrix[2][1] * TranslationMatrix1[1][0]) + (TransformationMatrix[2][2] * TranslationMatrix1[2][0]);
								  																													  
	TransformationMatrix[0][1] = (TransformationMatrix[0][0] * TranslationMatrix1[0][1]) + (TransformationMatrix[0][1] * TranslationMatrix1[1][1]) + (TransformationMatrix[0][2] * TranslationMatrix1[2][1]);
	TransformationMatrix[1][1] = (TransformationMatrix[1][0] * TranslationMatrix1[0][1]) + (TransformationMatrix[1][1] * TranslationMatrix1[1][1]) + (TransformationMatrix[1][2] * TranslationMatrix1[2][1]);
	TransformationMatrix[2][1] = (TransformationMatrix[2][0] * TranslationMatrix1[0][1]) + (TransformationMatrix[2][1] * TranslationMatrix1[1][1]) + (TransformationMatrix[2][2] * TranslationMatrix1[2][1]);
								  																													  
	TransformationMatrix[0][2] = (TransformationMatrix[0][0] * TranslationMatrix1[0][2]) + (TransformationMatrix[0][1] * TranslationMatrix1[1][2]) + (TransformationMatrix[0][2] * TranslationMatrix1[2][2]);
	TransformationMatrix[1][2] = (TransformationMatrix[1][0] * TranslationMatrix1[0][2]) + (TransformationMatrix[1][1] * TranslationMatrix1[1][2]) + (TransformationMatrix[1][2] * TranslationMatrix1[2][2]);
	TransformationMatrix[2][2] = (TransformationMatrix[2][0] * TranslationMatrix1[0][2]) + (TransformationMatrix[2][1] * TranslationMatrix1[1][2]) + (TransformationMatrix[2][2] * TranslationMatrix1[2][2]);
}

void Core::RotationMatrix::ApplyRotationToRectPoints(Vector<float>* Points, float Matrix[3][3])
{
	for (int i = 0; i < 4; i++)
	{
		float PointsToMultiply[3] = { Points[i].X, Points[i].Y, 1};
		float xRotated = (PointsToMultiply[0] * Matrix[0][0]) + (PointsToMultiply[1] * Matrix[0][1]) + (PointsToMultiply[2] * Matrix[0][2]);
		float yRotated = (PointsToMultiply[0] * Matrix[1][0]) + (PointsToMultiply[1] * Matrix[1][1]) + (PointsToMultiply[2] * Matrix[1][2]);
		Points[i] = Vector<float>(xRotated, yRotated);
	}
}
