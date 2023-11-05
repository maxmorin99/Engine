#include "Services/Scene.h"
#include "Services/SdlTileMap.h"

Core::Scene::Scene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH) :
	mName(name),
	mTileMap(new SdlTileMap(tilemapFile, srcTileW, srcTileH, tileCountW, tileCountH))
{
}

void Core::Scene::Load()
{
}

void Core::Scene::Destroy()
{
	mTileMap->Shutdown();
	delete mTileMap;
}
