#include "Services/SdlTileMap.h"
#include <SDL_image.h>

Core::SdlTileMap::SdlTileMap(SDL_Renderer* Renderer) :
	mRenderer(Renderer)
{
}

void Core::SdlTileMap::LoadTileset(const std::string& ImageFile, int TileW, int TileH, int Col, int Count)
{
	mTilesetTexture = IMG_LoadTexture(mRenderer, ImageFile.c_str());
}
