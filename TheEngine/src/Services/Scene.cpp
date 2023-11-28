#include "Services/Scene.h"
#include "Services/SdlTileMap.h"

Core::Scene::Scene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH) :
	mName(name),
	mSrcTileW(srcTileW),
	mSrcTileH(srcTileH),
	mTileCountW(tileCountW),
	mTileCountH(tileCountH)
{
	if (tilemapFile)
	{
		mTiledFile = tilemapFile;
	}
}

void Core::Scene::Load()
{
}

void Core::Scene::Destroy()
{
}
