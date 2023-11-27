#pragma once

#include "Services/Scene.h"

using namespace Core;

class MenuScene : public Scene
{
public:
	MenuScene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH);
	virtual void Load() override;
};