#include "Services/SdlTileMap.h"
#include <SDL_image.h>
#include <fstream>
#include "Engine/Engine.h"
#include "Engine/Core.h"

Core::SdlTileMap::SdlTileMap() :
	SdlTileMap("")
{
}

Core::SdlTileMap::SdlTileMap(const std::string& TiledFile) :
	mTiledFile(TiledFile)
{

}

void Core::SdlTileMap::SetTiledFile(const std::string& File)
{
	mTiledFile = File;
}

void Core::SdlTileMap::Draw()
{
	// draw each layer with his corresponding tilset, based on firstid and lastid of the tileset.
	for (auto& pair : mTilemap)
	{
		TLayer Layer = pair.second;
		for (int y = 0; y < Layer.size(); y++)
		{
			for (int x = 0; x < Layer[y].size(); x++)
			{
				int TileId = Layer[y][x];
				Tileset T = GetTilesetBasedOnTileId(TileId);
				
				Rect<int> Src = T.Sources[TileId];
				Rect<int> Dst(x * 32, y * 32, 32, 32);

				//Engine::GetGraphic().DrawTexture(T.ImageId, Src, Dst, 0.0, SDL_FLIP_NONE, Color::White);
			}
		}
	}
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
	std::string LayerName;

	std::string Line;
	while (std::getline(TiledFile, Line))
	{
		if (bInData)
		{
			// Récupérer le tableau 2d de int
			TLayer Layer;
			GetLayerData(TiledFile, Layer, LayerH);
			mTilemap[LayerName] = Layer;
			break;
		}

		if (!bInData && Line.find("<layer") != Line.npos && Line.find("name=") != Line.npos)
		{
			LayerName = GetLayerName(Line);
			if (LayerName == Name)
			{
				bInData = true;
				GetLayerSize(Line, &LayerW, &LayerH);
			}
		}
	}

	TiledFile.close();
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
			GetLayerValue(Line, WidthValueStartPos, WidthValueEndPos, OutW);
		}
	}

	// Height
	size_t HeightFlagStartPos = Line.find("height=\"");
	if (HeightFlagStartPos == Line.npos)
	{
		*OutH = 0;
	}
	else
	{
		size_t HeightValueStartPos = Line.find("\"", HeightFlagStartPos) + 1;
		size_t HeightValueEndPos = Line.find("\"", HeightValueStartPos);

		if (HeightValueStartPos == Line.npos || HeightValueEndPos == Line.npos)
		{
			*OutH = 0;
		}
		else
		{
			GetLayerValue(Line, HeightValueStartPos, HeightValueEndPos, OutH);
		}
	}
}

void Core::SdlTileMap::GetLayerValue(const std::string& Line, const size_t Begin, const size_t End, int* OutValue) const
{
	std::string WidthStr;
	for (int i = Begin; i < End; i++)
	{
		WidthStr += Line[i];
	}

	*OutValue = std::stoi(WidthStr);
}

void Core::SdlTileMap::GetLayerData(std::ifstream& TiledFile, TLayer& Out2DArray, int LayerH) const
{
	std::string Line;

	for (int y = 0; y < LayerH; y++)
	{
		std::getline(TiledFile, Line);
		Out2DArray.push_back(GetLineData(Line));
	}
	int bob = 0;
}

std::vector<int> Core::SdlTileMap::GetLineData(const std::string& Line) const
{
	std::vector<int> LineData;

	std::string TileIdStr;
	for (int j = 0; j < Line.size(); j++)
	{
		char c = Line[j];
		if (c == ',' || j == Line.size() - 1)
		{
			TileIdStr = j == Line.size() - 1 ? TileIdStr + c : TileIdStr;
			LineData.push_back(std::stoi(TileIdStr));
			TileIdStr.clear();
		}
		else
		{
			TileIdStr += c;
		}
	}
	return LineData;
}

Core::Tileset Core::SdlTileMap::GetTilesetBasedOnTileId(int TileId) const
{
	for (Tileset T : mTilesets)
	{
		if (TileId >= T.FirstId && TileId <= T.LastId)
		{
			return T;
		}
	}
	return Tileset();
}
