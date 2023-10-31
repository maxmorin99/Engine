#pragma once

#include <string>
#include "Engine/Core.h"

namespace Core
{
	class ITileMap
	{
	public:
		virtual void SetTiledFile(const std::string& File) = 0;
		virtual void AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count) = 0;
		virtual void AddLayer(const std::string& Name) = 0;
		virtual void Draw() = 0;
		virtual void Shutdown() = 0;
	};
}