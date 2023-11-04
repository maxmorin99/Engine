#include "Services/SdlTileMap.h"
#include <SDL_image.h>
#include <fstream>
#include "Engine/Engine.h"
#include "Engine/Core.h"

Core::SdlTileMap::SdlTileMap(const std::string& TiledFile, float SrcTileW, float SrcTileH, int TileCountW, int TileCountH) :
	mTiledFile(new std::string(TiledFile)),
	mTilesets(new std::vector<Tileset>),
	mTilemap(new TTilemap()),
	mObjectsRect(new std::vector<Rect<float>>()),
	mTileCount(TileCountW, TileCountH)
{
	int WindowW, WindowH = 0;
	Engine::GetGraphic().GetWindowSize(&WindowW, &WindowH);
	mInitialTileSize.X = SrcTileW;
	mInitialTileSize.Y = SrcTileH;
	mScaledTileSize.X = WindowW / mTileCount.X;
	mScaledTileSize.Y = WindowH / mTileCount.Y;
}

void Core::SdlTileMap::SetTiledFile(const std::string& File)
{
	mTiledFile = new std::string(File);
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
			GetLayerMatrix(TiledFile, Layer, LayerH);

			(*mTilemap)[LayerName] = Layer;
			break;
		}

		if (!bInData && Line.find("<layer") != Line.npos && Line.find("name=") != Line.npos)
		{
			LayerName = GetNameAttributeFromLine(Line);
			if (LayerName == Name)
			{
				bInData = true;
				GetLayerSize(Line, &LayerW, &LayerH);
			}
		}
	}

	TiledFile.close();
}

std::string Core::SdlTileMap::GetNameAttributeFromLine(const std::string& Line) const
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

void Core::SdlTileMap::GetLayerMatrix(std::ifstream& TiledFile, TLayer& Out2DArray, int LayerH) const
{
	std::string Line;

	for (int y = 0; y < LayerH; y++)
	{
		std::getline(TiledFile, Line);
		Out2DArray.push_back(GetLayerSingleArray(Line));
	}
}

std::vector<int> Core::SdlTileMap::GetLayerSingleArray(const std::string& Line) const
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

void Core::SdlTileMap::GetLayerSize(const std::string& Line, int* OutW, int* OutH) const
{
	if (!OutW || !OutH) return;

	// Width
	std::string ValueStr = GetStringValueFromFlag(Line, "width=");
	*OutW = std::stoi(ValueStr);

	// Height
	ValueStr = GetStringValueFromFlag(Line, "height=");
	*OutH = std::stoi(ValueStr);
}

std::string Core::SdlTileMap::GetStringValueFromFlag(const std::string& Line, const std::string& Flag) const
{
	size_t FlagStartPos = Line.find(Flag);
	if (FlagStartPos == Line.npos)
	{
		return std::string("0");
	}

	size_t DataStartPos = Line.find("\"", FlagStartPos) + 1;
	size_t DataEndPos = Line.find("\"", DataStartPos);

	if (DataStartPos == Line.npos || DataEndPos == Line.npos)
	{
		return std::string("0");
	}
	else
	{
		return GetDelimitedStringValue(Line, DataStartPos, DataEndPos);
	}
}

std::string Core::SdlTileMap::GetDelimitedStringValue(const std::string& Line, const size_t Begin, const size_t End) const
{
	std::string WidthStr;
	for (int i = Begin; i < End; i++)
	{
		WidthStr += Line[i];
	}

	return WidthStr;
}

void Core::SdlTileMap::AddObjectLayer(const std::string& Name)
{
	std::ifstream TiledFile(*mTiledFile);
	if (!TiledFile.is_open()) return;

	bool bInData = false;
	std::string LayerName;
	std::string Line;

	while (std::getline(TiledFile, Line))
	{
		if (bInData)
		{
			std::vector<Rect<float>> mRects;
			ConstructRectsFromObjectLayer(TiledFile, Line, mRects);
			break;
		}
		if (!bInData && Line.find("<objectgroup") != Line.npos && Line.find("name=") != Line.npos)
		{
			LayerName = GetNameAttributeFromLine(Line);

			if (LayerName == Name)
			{
				bInData = true;
			}
		}
	}

	TiledFile.close();
}

void Core::SdlTileMap::GetObjectData(const std::string& Line, float* X, float* Y, float* W, float* H) const
{
	if (!X || !Y || !W || !H) return;

	*X = (int)std::stof(GetStringValueFromFlag(Line, "x="));
	*Y = (int)std::stof(GetStringValueFromFlag(Line, "y="));
	*W = std::stof(GetStringValueFromFlag(Line, "width="));
	*H = std::stof(GetStringValueFromFlag(Line, "height="));

	// Scale values with tile size to match the tile's proportions
	float ScaledRatioTileX = mScaledTileSize.X / mInitialTileSize.X;
	float ScaledRatioTiledY = mScaledTileSize.Y / mInitialTileSize.Y;
	*X *= ScaledRatioTileX;
	*Y *= ScaledRatioTiledY;
	*W *= ScaledRatioTileX;
	*H *= ScaledRatioTiledY;
}

void Core::SdlTileMap::ConstructRectsFromObjectLayer(std::ifstream& TiledFile, const std::string& FirstLine, std::vector<Rect<float>>& OutRects)
{
	std::string Line = FirstLine;

	while (Line.find("</objectgroup>") == Line.npos)
	{
		Rect<float> ObjectRect;
		GetObjectData(Line, &ObjectRect.X, &ObjectRect.Y, &ObjectRect.W, &ObjectRect.H);
		
		if (ObjectRect.W != 0 || ObjectRect.H != 0)
		{
			mObjectsRect->push_back(ObjectRect);
		}
		std::getline(TiledFile, Line);
	}
}

void Core::SdlTileMap::Draw()
{
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
				Rect<int> Dst(x * mScaledTileSize.X, y * mScaledTileSize.Y, mScaledTileSize.X, mScaledTileSize.Y);

				Engine::GetGraphic().DrawTexture(T._ImageId, Src, Dst, 0.0, Flip::None, Color::White);
			}
		}
	}

	for (auto& R : *mObjectsRect)
	{
		Engine::GetGraphic().DrawRectF(false, &R, Color::Green);
	}
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

	mObjectsRect->clear();

	delete mTiledFile;
	delete mTilesets;
	delete mTilemap;
	delete mObjectsRect;
}
