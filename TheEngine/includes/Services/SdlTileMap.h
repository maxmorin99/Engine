#pragma once

#include "Interfaces/ITileMap.h"
#include <vector>
#include <unordered_map>
#include <utility.h>

#define TILE_WIDTH_COUNT 25
#define TILE_HEIGHT_COUNT 19

struct SDL_Texture;
struct SDL_Renderer;

typedef std::vector<Core::Rect<int>> TTileset;
typedef std::vector<std::vector<int>> TLayer;
typedef std::unordered_map<std::string, TLayer> TTilemap;

namespace Core
{
	struct Tileset
	{
		int _FirstId{ 0 };
		int _LastId{ 0 };
		size_t _ImageId{ 0 };
		TTileset _Sources;
	};

	/** Object representing a collision in the tilemap */
	struct TilemapObject
	{
		TilemapObject(const std::string& Id, Rect<float>& Rect) :
			_Rect(Rect), _Id(Id) {};

		Rect<float> _Rect;
		std::string _Id;
	};

	class SdlTileMap : public ITileMap
	{
	public:
		SdlTileMap(const std::string& TiledFile, float SrcTileW, float SrcTileH, int TileCountW, int TileCountH);
		~SdlTileMap() = default;
		virtual void AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count) override;
		virtual void AddLayer(const std::string& Name) override;
		virtual void AddObjectLayer(const std::string& Name) override;
		virtual void SetTiledFile(const std::string& File) override;
		virtual void Draw() override;
		virtual void Shutdown() override;

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