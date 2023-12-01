#pragma once

#include "Services/Scene.h"

using namespace Core;

class SecondScene : public Scene
{
public:
	SecondScene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH);
	virtual void Load() override;
};