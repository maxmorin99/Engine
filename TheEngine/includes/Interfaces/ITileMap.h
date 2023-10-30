#pragma once

#include <string>

namespace Core
{
	class ITileMap
	{
	public:
		virtual void LoadTileset(const std::string& ImageFile, int TileW, int TileH, int Col, int Count) = 0;
	};
}