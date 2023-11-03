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
	mTiledFile(new std::string(TiledFile)),
	mTilesets(new std::vector<Tileset>),
	mTilemap(new TTilemap())
{
}

void Core::SdlTileMap::SetTiledFile(const std::string& File)
{
	mTiledFile = new std::string(File);
}

void Core::SdlTileMap::Draw()
{
	int WindowW, WindowH = 0;
	Engine::GetGraphic().GetWindowSize(&WindowW, &WindowH);
	int TileW = WindowW / TILE_WIDTH_COUNT;
	int TileH = WindowH / TILE_HEIGHT_COUNT;

	size_t bob = 0;
	// draw each layer with his corresponding tilset, based on firstid and lastid of the tileset.
	for (auto& pair : *mTilemap)
	{
		TLayer Layer = pair.second;
		for (int y = 0; y < Layer.size(); y++)
		{
			for (int x = 0; x < Layer[y].size(); x++)
			{
				int TileId = Layer[y][x];
				if (TileId == 0) continue;

				Tileset T = GetTilesetBasedOnTileId(TileId);
				TileId -= T._FirstId - 1;
				if (TileId >= T._Sources.size()) continue;

				Rect<int> Src = T._Sources[TileId - 1];
				Rect<int> Dst(x * TileW, y * TileH, TileW, TileH);
				
				Engine::GetGraphic().DrawTexture(T._ImageId, Src, Dst, 0.0, Flip::None, Color::White);
			}
		}
	}
}

void Core::SdlTileMap::AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count)
{
	if (!mTilesets) return;

	size_t ImageId = Engine::GetGraphic().LoadTexture(ImageFile.c_str());
	if (ImageId == -1) return;

	Tileset T;
	T._ImageId = ImageId;
	T._FirstId = FirstId;
	T._LastId = (FirstId + Count) - 1;
	
	for (int i = 0; i < Count; i++)
	{
		int Y = i / Col;
		int X = i - Y * Col;

		Rect<int> Tile(X * TileW, Y * TileH, TileW, TileH);
		T._Sources.push_back(Tile);
	}

	mTilesets->push_back(T);
}

void Core::SdlTileMap::AddLayer(const std::string& Name)
{
	std::ifstream TiledFile(*mTiledFile);
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
			

			(*mTilemap)[LayerName] = Layer;
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
	for (Tileset T : *mTilesets)
	{
		if (TileId >= T._FirstId && TileId <= T._LastId)
		{
			return T;
		}
	}

	return Tileset();
}

void Core::SdlTileMap::Shutdown()
{
	// récupérer chaque TLayer de la Tilemap
	// clear le 2d array TLayer

	// clear chaque tileset de mTilesets

	for (auto& Pair : *mTilemap)
	{
		TLayer L = Pair.second;
		for (int i = 0; i < L.size(); i++)
		{
			L[i].clear();
		}
		L.clear();
	}
	mTilemap->clear();

	for (auto& Tileset : *mTilesets)
	{
		TTileset Sources = Tileset._Sources;
		Sources.clear();
	}
	mTilesets->clear();

	delete mTiledFile;
	delete mTilesets;
	delete mTilemap;
}
