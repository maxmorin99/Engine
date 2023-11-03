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

	class SdlTileMap : public ITileMap
	{
	public:
		SdlTileMap();
		SdlTileMap(const std::string& TiledFile);
		~SdlTileMap() = default;
		virtual void AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count) override;
		virtual void AddLayer(const std::string& Name) override;
		virtual void SetTiledFile(const std::string& File) override;
		virtual void Draw() override;
		virtual void Shutdown() override;
		

	private:
		std::string* mTiledFile = nullptr;

		/** List of all the different tilesets */
		std::vector<Tileset>* mTilesets = nullptr;

		/** Dict of str -> TLayer */
		TTilemap* mTilemap = nullptr;


		std::string GetLayerName(const std::string& Line) const;

		/** Get width and height of the 2d array of int values of the layer */
		void GetLayerSize(const std::string& Line, int* OutW, int* OutH) const;

		/** Get an integer value of a string from idx Begin to idx End */
		void GetLayerValue(const std::string& Line, const size_t Begin, const size_t End, int* OutValue) const;

		/** Get the 2d array of int values of the layer */
		void GetLayerData(std::ifstream& TiledFile, TLayer& Out2DArray, int LayerH) const;

		/** Get list of int values of a single line */
		std::vector<int> GetLineData(const std::string& Line) const;

		Tileset GetTilesetBasedOnTileId(int TileId) const;
	};
}