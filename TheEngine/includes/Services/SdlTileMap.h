#pragma once

#include "Interfaces/ITileMap.h"

struct SDL_Texture;
struct SDL_Renderer;

namespace Core
{
	class SdlTileMap : public ITileMap
	{
	public:
		SdlTileMap(SDL_Renderer* Renderer);
		virtual void LoadTileset(const std::string& ImageFile, int TileW, int TileH, int Col, int Count) override;

	private:
		SDL_Renderer* mRenderer = nullptr;
		SDL_Texture* mTilesetTexture = nullptr;
	};
}