#pragma once



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

	template <typename T>
	struct Vector
	{
	public:
		Vector(T InX, T InY)
			: X(InX), Y(InY) {}
		T X;
		T Y;


		static const Vector<float> ZeroVectorF;
		static const Vector<int> ZeroVector;
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