#pragma once

#include <math.h>
#include <string>
#include <vector>


namespace Core
{
#define ASSET_PATH "./Assets/"

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
		Rect() :
			X(0), Y(0), W(0), H(0) {}
		Rect(T InX, T InY, T InW, T InH)
			: X(InX), Y(InY), W(InW), H(InH) {}

		T X;
		T Y;
		T W;
		T H;
	};



	/** Vector ---------------------------------------------------- */

	template <typename T>
	struct Vector
	{
	public:
		Vector() : X(0), Y(0) {}
		Vector(T InX, T InY) : X(InX), Y(InY) {}

		
		T X;
		T Y;
	

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

		Vector<T>& operator -= (const Vector& Other)
		{
			X -= Other.X;
			Y -= Other.Y;
			return *this;
		}

		Vector<T>& operator -= (const T Scalar)
		{
			X -= Scalar;
			Y -= Scalar;
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


		/* return a vector with X(0) and Y (0) */
		static Vector<T> ZeroVector() 
		{
			return Vector<T>(0, 0);
		}

		/* return the magnitude of the vector */
		T Length() const
		{
			return static_cast<T>(sqrt(pow(X, 2) + pow(Y, 2)));
		}

		/* return the normalize version of the vector */
		Vector<T> GetNormalized() const
		{
			if (Length() != 0.f)
			{
				T CurrentLength = Length();
				T NormalizedX = X / CurrentLength;
				T NormalizedY = Y / CurrentLength;
				return Vector<T>(NormalizedX, NormalizedY);
			}
			return *this;
		}

		/* Normalize the vector */
		void Normalize()
		{
			*this = Vector<T>::ZeroVector();
		}
	};

	/** 
	*	Flip data for a texture. 
	*	H = Wether or not the texture flips horizontally 
	*	V = Wether or not the texture flips vertically 
	*/
	struct Flip
	{
		Flip(bool InH, bool InV) : H(InH), V(InV) {}
			
		bool H;
		bool V;

		static const Flip None;
	};

	
	/** Color for SDL objects (rect, lines, textures, ...) */
	struct Color
	{
		typedef unsigned char uchar;

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

	struct TextureData
	{
		TextureData(const std::string& Texture, int R, int C, int Idx, uint8_t Pad) :
			File(Texture), Rows(R), Col(C), Index(Idx), Padding(Pad) {}

		/* File path of the texture */
		std::string File;

		/* Rows count */
		int Rows;

		/* Columns count */
		int Col;

		/* Index in the sprite sheet of the desired sprite */
		int Index;

		/* Padding in Px between each sprites */
		uint8_t Padding;

		static TextureData& Empty()
		{
			return TextureData("", 0, 0, 0, 0);
		}
	};
}