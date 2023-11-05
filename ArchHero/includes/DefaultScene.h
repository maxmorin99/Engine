#pragma once

#include "Services/Scene.h"

using namespace Core;

class DefaultScene : public Scene
{
public:
	DefaultScene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH);
	virtual void Load() override;
};