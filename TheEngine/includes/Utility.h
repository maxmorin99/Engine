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

	/**
	* @brief Represents a rectangular region with position and dimensions.
	*
	* @tparam T The type of the components (X, Y, W, H).
	*/
	template <typename T>
	struct Rect
	{

		/**
		* @brief Default constructor, initializes all components to zero.
		*/
		Rect() :
			X(0), Y(0), W(0), H(0) {}

		/**
		* @brief Parameterized constructor, initializes components with provided values.
		*
		* @param InX The X-coordinate of the top-left corner.
		* @param InY The Y-coordinate of the top-left corner.
		* @param InW The width of the rectangle.
		* @param InH The height of the rectangle.
		*/
		Rect(T InX, T InY, T InW, T InH)
			: X(InX), Y(InY), W(InW), H(InH) {}

		/** X-coordinate of the top-left corner */
		T X;

		/** Y-coordinate of the top-left corner */
		T Y;

		/** Width of the rectangle */
		T W;

		/** Height of the rectangle */
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

		/**
		* @brief Default constructor, initializes X and Y to zero.
		*/
		Vector() : X(0), Y(0) {}

		/**
		* @brief Parameterized constructor, initializes X and Y with provided values.
		*
		* @param InX The value to set for the X component.
		* @param InY The value to set for the Y component.
		*/
		Vector(T InX, T InY) : X(InX), Y(InY) {}

		/**
		* @brief Calculates the rotation angle (in radians) from the positive X-axis.
		*
		* @return The rotation angle from the positive X-axis.
		*/
		float GetRotationFromX()
		{
			return (float)(acos(X / sqrt(pow(X, 2) + pow(Y, 2))));
		}

		/** Vector components */
		T X;
		T Y;
	

		/** Operator overload ----------------------------------------- */


		/**
		* @brief Addition operator overload.
		*
		* @param Other The vector to add.
		* @return The result of the addition operation.
		*/
		Vector<T> operator + (const Vector& Other) const
		{
			return Vector<T>(X + Other.X, Y + Other.Y);
		}

		/**
		* @brief Compound addition operator overload.
		*
		* @param Other The vector to add.
		* @return A reference to the modified vector after addition.
		*/
		Vector<T>& operator += (const Vector& Other)
		{
			X += Other.X;
			Y += Other.Y;
			return *this;
		}

		/**
		* @brief Subtraction operator overload.
		*
		* @param Other The vector to subtract.
		* @return The result of the subtraction operation.
		*/
		Vector<T> operator - (const Vector& Other) const
		{
			return Vector<T>(X - Other.X, Y - Other.Y);
		}

		/**
		* @brief Negation operator overload.
		*
		* @return The negated vector.
		*/
		Vector<T> operator - () const
		{
			return Vector<T>(-X, -Y);
		}

		/**
		* @brief Compound subtraction operator overload.
		*
		* @param Other The vector to subtract.
		* @return A reference to the modified vector after subtraction.
		*/
		Vector<T>& operator -= (const Vector& Other)
		{
			X -= Other.X;
			Y -= Other.Y;
			return *this;
		}

		/**
		* @brief Scalar subtraction operator overload.
		*
		* @param Scalar The scalar value to subtract.
		* @return A reference to the modified vector after subtraction.
		*/
		Vector<T>& operator -= (const T Scalar)
		{
			X -= Scalar;
			Y -= Scalar;
			return *this;
		}

		/**
		* @brief Equality comparison operator overload.
		*
		* @param Other The vector to compare.
		* @return A boolean indicating equality.
		*/
		bool operator == (const Vector& Other) const
		{
			return this->X == Other.X && this->Y == Other.Y;
		}

		/**
		* @brief Inequality comparison operator overload.
		*
		* @param Other The vector to compare.
		* @return A boolean indicating inequality.
		*/
		bool operator != (const Vector& Other) const
		{
			return this->X != Other.X || this->Y != Other.Y;
		}

		/**
		* @brief Assignment operator overload.
		*
		* @param Other The vector to assign.
		* @return A reference to the modified vector after assignment.
		*/
		Vector<T>& operator = (const Vector& Other)
		{
			this->X = Other.X;
			this->Y = Other.Y;
			return *this;
		}

		/**
		* @brief Scalar multiplication operator overload.
		*
		* @param Scalar The scalar value to multiply.
		* @return The result of the multiplication operation.
		*/
		Vector<T> operator * (T Scalar) const
		{
			return Vector<T>(X * Scalar, Y * Scalar);
		}

		/**
		* @brief Scalar division operator overload.
		*
		* @param Scalar The scalar value to divide.
		* @return The result of the division operation.
		*/
		Vector<T> operator / (T Scalar) const
		{
			return Vector<float>(X / Scalar, Y / Scalar);
		}


		/**
		* @brief Returns a vector with X(0) and Y(0).
		*
		* @return The zero vector.
		*/
		static Vector<T> ZeroVector() 
		{
			return Vector<T>(0, 0);
		}

		/**
		* @brief Calculates the magnitude (length) of the vector.
		*
		* @return The magnitude of the vector.
		*/
		T Length() const
		{
			return static_cast<T>(sqrt(pow(X, 2) + pow(Y, 2)));
		}

		/**
		* @brief Returns the normalized version of the vector.
		*
		* @return The normalized vector.
		*/
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

		/**
		* @brief Normalizes the vector in-place.
		*/
		void Normalize()
		{
			*this = Vector<T>::ZeroVector();
		}

		/**
		* @brief Calculates the distance between two points represented by vectors.
		*
		* @param Point1 The first point.
		* @param Point2 The second point.
		* @return The distance between the two points.
		*/
		static float Dist(const Vector<T>& Point1, const Vector<T>& Point2)
		{
			float DistX = abs(Point1.X - Point2.X);
			float DistY = abs(Point1.Y - Point2.Y);
			return (float)sqrt(pow(DistX, 2) + pow(DistY, 2));
		}

		/**
		* @brief Calculates the dot product of this vector with another vector.
		*
		* @param Other The other vector.
		* @return The dot product of the two vectors.
		*/
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

	/**
	* @brief Represents data for a texture, including details for sprite sheet handling.
	*/
	struct TextureData
	{

		/**
		* @brief Constructor for initializing texture data.
		*
		* @param Texture The file path of the texture.
		* @param R The number of rows in the sprite sheet.
		* @param C The number of columns in the sprite sheet.
		* @param Idx The index in the sprite sheet of the desired sprite.
		* @param Pad Padding in pixels between each sprite.
		*/
		TextureData(const std::string& Texture, int R, int C, int Idx, uint8_t Pad) :
			File(Texture), Rows(R), Col(C), Index(Idx), Padding(Pad) {}

		/** File path of the texture */
		std::string File;

		/** Rows count */
		int Rows;

		/** Columns count */
		int Col;

		/** Index in the sprite sheet of the desired sprite */
		int Index;

		/** Padding in Px between each sprites */
		uint8_t Padding;

		/**
		* @brief Creates an empty TextureData instance with default values.
		*
		* @return An empty TextureData instance.
		*/
		static TextureData Empty()
		{
			return TextureData("", 0, 0, 0, 0);
		}
	};


	/** Collision ------------------------------------------------- */

	/**
	* @brief Enumerates collision shapes.
	*/
	enum class ECollisionShape : uint8_t
	{
		Circle = 0,
		Rectangle = 1,
		Line = 2,

		MAX
	};

	/**
	* @brief Enumerates collision channels for categorizing collision objects.
	*/
	enum class ECollisionChannel : uint8_t
	{
		Player = 0,
		Enemy = 1,
		Projectile = 2,
		World = 3,
		UI = 4,

		MAX
	};

	/**
	* @brief Enumerates collision responses for determining how collisions should be handled.
	*/
	enum class ECollisionResponse : uint8_t
	{
		Block = 0,
		Overlap = 1,
		Ignore = 2,

		MAX
	};

	/**
	* @brief Enumerates collision sides for indicating the side of a collision.
	*/
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