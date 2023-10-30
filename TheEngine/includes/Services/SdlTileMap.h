#pragma once

#include "Interfaces/ITileMap.h"
#include <vector>
#include <unordered_map>
#include <utility.h>

struct SDL_Texture;
struct SDL_Renderer;

typedef std::vector<Core::Rect<int>> TTileset;
typedef std::vector<std::vector<int>> TLayer;
typedef std::unordered_map<std::string, TLayer> TTilemap;

namespace Core
{
	struct Tileset
	{
		int FirstId{ 0 };
		int LastId{ 0 };
		size_t ImageId{ 0 };
		TTileset Sources;
	};

	class SdlTileMap : public ITileMap
	{
	public:
		SdlTileMap();
		SdlTileMap(const std::string& TiledFile);
		virtual void AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count) override;
		virtual void AddLayer(const std::string& Name) override;
		virtual void SetTiledFile(const std::string& File) override;

	private:
		std::string mTiledFile;

		/** List of all the different tilesets */
		std::vector<Tileset> mTilesets;

		TTilemap mTilemap;

		std::string GetLayerName(const std::string& Line) const;
		void GetLayerSize(const std::string& Line, int* OutW, int* OutH) const;
		void GetLayerWidth(const std::string& Line, const size_t WidthBegin, const size_t WidthEnd, int* OutW) const;
	};
}