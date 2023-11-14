#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include "Utility.h"

#define TILE_WIDTH_COUNT 25
#define TILE_HEIGHT_COUNT 19

namespace Core
{
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

	class TilemapComponent : public Component, public IDrawable
	{
	public:
		TilemapComponent(Object* Owner);
		virtual void Draw() override;
		void SetTilemapData(const std::string& TiledFile, float SrcTileW, float SrcTileH, int TileCountW, int TileCountH);
		void AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count);
		void AddLayer(const std::string& Name);
		void AddObjectLayer(const std::string& Name);
		void SetTiledFile(const std::string& File);
		virtual void Destroy() override;
		TTilemap GetTilemap() const;

	private:

		std::string* mTiledFile = nullptr;

		/** List of all the different tilesets */
		std::vector<Tileset>* mTilesets = nullptr;

		/** Dict of str -> TLayer */
		TTilemap* mTilemap = nullptr;

		/** List of collision objects of the tilemap */
		std::vector<TilemapObject>* mTilemapObjects = nullptr;

		/** Size of tile scaled on screen */
		Vector<float> mScaledTileSize = Vector<float>::ZeroVector();

		/** Size of tile directly from Tiled */
		Vector<float> mInitialTileSize = Vector<float>::ZeroVector();

		/** Tile count of the tileMap
		*	X represents the number of tile from left to right
		*	y represents the number of tile from top to bottom
		*/
		Vector<int> mTileCount = Vector<int>::ZeroVector();



		/** Get width and height of the 2d array of int values of the layer */
		void GetLayerSize(const std::string& Line, int* OutW, int* OutH) const;

		/** Get a substring of a string from idx Begin to idx End */
		std::string GetDelimitedStringValue(const std::string& Line, const size_t Begin, const size_t End) const;

		/** Get the 2d array of int values of the layer */
		void GetLayerMatrix(std::ifstream& TiledFile, TLayer& Out2DArray, int LayerH) const;

		/** Get list of int values of a single line */
		std::vector<int> GetLayerSingleArray(const std::string& Line) const;

		/** Get all object properties(x, y, width, height) from a str line */
		void GetObjectData(const std::string& Line, std::string& OutId, float* X, float* Y, float* W, float* H) const;

		/** Get the value (str) associated to the flag. Ex: Flag = "width", returned value = "32" */
		std::string GetStringValueFromFlag(const std::string& Line, const std::string& Flag) const;

		/** Construct all rect from object layer (for collisions) */
		void ConstructRectsFromObjectLayer(std::ifstream& TiledFile, const std::string& FirstLine);

		std::string GetNameAttributeFromLine(const std::string& Line) const;
		Tileset GetTilesetBasedOnTileId(int TileId) const;

	public:
		std::vector<TilemapObject> GetTilemapObjects() const;
	};
}