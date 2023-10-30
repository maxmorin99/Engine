#include "Services/SdlTileMap.h"
#include <SDL_image.h>
#include <fstream>
#include "Engine/Engine.h"

Core::SdlTileMap::SdlTileMap() :
	SdlTileMap("")
{
}

Core::SdlTileMap::SdlTileMap(const std::string& TiledFile) :
	mTiledFile(TiledFile)
{

}

void Core::SdlTileMap::AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count)
{
	size_t ImageId = Engine::GetGraphic().LoadTexture(ImageFile.c_str());
	if (ImageId == -1) return;

	Tileset T;
	T.ImageId = ImageId;
	T.FirstId = FirstId;
	T.LastId = (FirstId + Count) - 1;
	
	for (int i = 0; i < Count; i++)
	{
		int Y = i / Col;
		int X = i - Y * Col;

		Rect<int> Tile(X * TileW, Y * TileH, TileW, TileH);
		T.Sources.push_back(Tile);
	}

	mTilesets.push_back(T);
}

void Core::SdlTileMap::AddLayer(const std::string& Name)
{
	std::ifstream TiledFile(mTiledFile);
	if (!TiledFile.is_open()) return;

	bool bInData = false;
	int LayerW = 0;
	int LayerH = 0;

	std::string Line;
	while (std::getline(TiledFile, Line))
	{
		if (bInData)
		{
			// Récupérer le tableau 2d de int
			
		}

		if (!bInData && Line.find("<layer") != Line.npos && Line.find("name=") != Line.npos)
		{
			std::string LayerName = GetLayerName(Line);
			if (LayerName == Name)
			{
				bInData = true;
				int LayerW = 0;
				int LayerH = 0;
				GetLayerSize(Line, &LayerW, &LayerH);
			}
		}
	}
}

void Core::SdlTileMap::SetTiledFile(const std::string& File)
{
	mTiledFile = File;
}

std::string Core::SdlTileMap::GetLayerName(const std::string& Line) const
{
	size_t NameFlagStartPos = Line.find("name=\"");
	size_t NameStartPos = Line.find("\"", NameFlagStartPos) + 1;
	size_t NameEndPos = Line.find("\"", NameStartPos);

	std::string Name;
	for (int i = NameStartPos; i < NameEndPos; i++)
	{
		Name += Line[i];
	}

	return Name;
}

void Core::SdlTileMap::GetLayerSize(const std::string& Line, int* OutW, int* OutH) const
{
	if (!OutW || !OutH) return;

	// Width
	size_t WidthFlagStartPos = Line.find("width=\"");
	if (WidthFlagStartPos == Line.npos)
	{
		*OutW = 0;
	}
	else
	{
		size_t WidthValueStartPos = Line.find("\"", WidthFlagStartPos) + 1;
		size_t WidthValueEndPos = Line.find("\"", WidthValueStartPos);

		if (WidthValueStartPos == Line.npos || WidthValueEndPos == Line.npos)
		{
			*OutW = 0;
		}
		else
		{
			GetLayerWidth(Line, WidthValueStartPos, WidthValueEndPos, OutW);
		}
	}

}

void Core::SdlTileMap::GetLayerWidth(const std::string& Line, const size_t WidthBegin, const size_t WidthEnd, int* OutW) const
{
	std::string WidthStr;
	for (int i = WidthBegin; i < WidthEnd; i++)
	{
		WidthStr += Line[i];
	}

	*OutW = std::stoi(WidthStr);
}

