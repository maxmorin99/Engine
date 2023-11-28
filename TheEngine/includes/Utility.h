#pragma once

#include <math.h>
#include <string>
#include <vector>
#include <unordered_map>


namespace Core
{
#define ASSET_PATH "./Assets/"

#define PI 3.14159265358979323846264338327950288419716939937510582

	
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


	/** Tilemap ------------------------------------------ */

	struct Tile
	{
		float X;
		float Y;
		float W;
		float H;
		std::string ParentLayerName;
	};

	/** List of sources rect of a tileset image */
	typedef std::vector<Rect<int>> TTileset;

	/** 2D list of int representing a tile layer */
	typedef std::vector<std::vector<int>> TLayer;

	struct Layer
	{
		std::string _Name;
		TLayer _Layer;
		std::vector<Tile> _Tiles;
	};

	/** Map of all the tile layers */
	typedef std::unordered_map<std::string, Layer> TTilemap;

	/** Data that represents a tileset */
	struct Tileset
	{
		/** The minimal id valid for this tileset */
		int _FirstId{ 0 };
		/** The maximal id valid for this tileset */
		int _LastId{ 0 };
		/** Id of the image loaded by graphic service */
		size_t _ImageId{ 0 };
		/** List of source rect for this tileset image */
		TTileset _Sources;
	};

	/** Object representing a collision in the tilemap */
	struct TilemapObject
	{
		TilemapObject(const std::string& Id, Rect<float>& Rect) :
			_Rect(Rect), _Id(Id) {};

		/** Rectangle representinf the object position and dimension */
		Rect<float> _Rect;
		/** Id of the object */
		std::string _Id;
	};


	/** Vector ---------------------------------------------------- */

	template <typename T>
	struct Vector
	{
	public:
		Vector() : X(0), Y(0) {}
		Vector(T InX, T InY) : X(InX), Y(InY) {}
		float GetRotationFromX()
		{
			return (float)(acos(X / sqrt(pow(X, 2) + pow(Y, 2))));
		}

		
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

		Vector<T> operator - (const Vector& Other) const
		{
			return Vector<T>(X - Other.X, Y - Other.Y);
		}

		Vector<T> operator - () const
		{
			return Vector<T>(-X, -Y);
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

		bool operator != (const Vector& Other) const
		{
			return this->X != Other.X || this->Y != Other.Y;
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

		/* Distance between Point1 and Point2 */
		static float Dist(const Vector<T>& Point1, const Vector<T>& Point2)
		{
			float DistX = abs(Point1.X - Point2.X);
			float DistY = abs(Point1.Y - Point2.Y);
			return (float)sqrt(pow(DistX, 2) + pow(DistY, 2));
		}

		T Dot(const Vector<T>& Other) const
		{
			return X * Other.X + Y * Other.Y;
		}
	};


	/** Color  ---------------------------------------------------- */


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


	/** Texture --------------------------------------------------- */

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

		static TextureData Empty()
		{
			return TextureData("", 0, 0, 0, 0);
		}
	};


	/** Collision ------------------------------------------------- */

	enum class ECollisionShape : uint8_t
	{
		Circle = 0,
		Rectangle = 1,
		Line = 2,

		MAX
	};

	enum class ECollisionChannel : uint8_t
	{
		Player = 0,
		Enemy = 1,
		Projectile = 2,
		World = 3,
		UI = 4,

		MAX
	};

	enum class ECollisionResponse : uint8_t
	{
		Block = 0,
		Overlap = 1,
		Ignore = 2,

		MAX
	};

	enum class ECollisionSide
	{
		Undefined = 0,
		Left,
		Right,
		Top,
		Bot,

		MAX
	};
}