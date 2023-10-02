#pragma once

#include <math.h>


namespace Core
{
#define ASSET_PATH "../ArchHero/Assets/"

	/** Enum for debug text color in console or files */
	enum class ConsoleColor
	{
		Black,
		Blue,
		Green,
		Cyan,
		Red,
		Purple,
		Yellow,
		White,
		Grey,

		MAX
	};

	template <typename T>
	struct Rect
	{
		Rect(T InX, T InY, T InW, T InH)
			: X(InX), Y(InY), W(InW), H(InH) {}
	public:
		T X;
		T Y;
		T W;
		T H;
	};

	/* Vector */

	template <typename T>
	struct Vector
	{
	public:
		Vector(T InX, T InY) :
			X(InX), Y(InY) {}

		T X;
		T Y;

		static Vector<T> ZeroVector(){
			return Vector<T>(0, 0);
		}

		/** Operator overload ----------------------------------------- */

		Vector<T> operator + (const Vector& Other) const
		{
			return Vector<T>(X + Other.X, Y + Other.Y);
		}

		Vector<T>& operator += (const Vector& Other)
		{
			X += Other.X;
			Y += Other.Y;
			return *this;
		}

		bool operator == (const Vector& Other) const
		{
			return this->X == Other.X && this->Y == Other.Y;
		}

		Vector<T>& operator = (const Vector& Other)
		{
			this->X = Other.X;
			this->Y = Other.Y;
			return *this;
		}

		Vector<T> operator * (T Scalar) const
		{
			return Vector<T>(X * Scalar, Y * Scalar);
		}

		Vector<T> operator / (T Scalar) const
		{
			return Vector<float>(X / Scalar, Y / Scalar);
		}

		
		T Length() const
		{
			return static_cast<T>(sqrt(pow(X, 2) + pow(Y, 2)));
		}

		Vector<T> GetNormalized() const
		{
			T CurrentLength = Length();
			T NormalizedX = X / CurrentLength;
			T NormalizedY = Y / CurrentLength();
			return Vector<T>(NormalizedX, NormalizedY);
		}

		//void SetLength();
	};

	/** 
	*	Flip data for a texture. 
	*	H = Wether or not the texture flips horizontally 
	*	V = Wether or not the texture flips vertically 
	*/
	struct Flip
	{
		Flip(bool InH, bool InV) :
			H(InH), V(InV) {}
		bool H;
		bool V;

		static const Flip None;
	};

	/** Color for SDL objects (rect, lines, textures, ...) */
	typedef unsigned char uchar;
	struct Color
	{
		Color(uchar Red, uchar Green, uchar Blue, uchar Alpha)
			: R(Red), G(Green), B(Blue), A(Alpha) {}
		uchar R;
		uchar G;
		uchar B;
		uchar A;

		static const Color Black;
		static const Color Blue;
		static const Color Green;
		static const Color Cyan;
		static const Color Red;
		static const Color Purple;
		static const Color Yellow;
		static const Color White;
		static const Color Grey;
	};
}