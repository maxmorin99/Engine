#include "Components/TilemapComponent.h"
#include <fstream>

Core::TilemapComponent::TilemapComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::TilemapComponent::AddTileset(const std::string& ImageFile, int FirstId, int TileW, int TileH, int Col, int Count)
{
	if (!mTilesets) return;

	size_t ImageId = Graphic().LoadTexture(ImageFile.c_str());
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

void Core::TilemapComponent::AddLayer(const std::string& Name)
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
			TLayer tLayer;
			GetLayerMatrix(TiledFile, tLayer, LayerH);

			/** Create Layer data structure */
			Layer layer;
			layer._Name = Name;
			layer._Layer = tLayer;
			layer._Tiles = GetListOfTileForLayer(layer);

			(*mTilemap)[LayerName] = layer;
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

std::string Core::TilemapComponent::GetNameAttributeFromLine(const std::string& Line) const
{
	size_t NameFlagStartPos = Line.find("name=\"");
	size_t NameStartPos = Line.find("\"", NameFlagStartPos) + 1;
	size_t NameEndPos = Line.find("\"", NameStartPos);

	std::string Name;
	for (size_t i = NameStartPos; i < NameEndPos; i++)
	{
		Name += Line[i];
	}

	return Name;
}

void Core::TilemapComponent::GetLayerMatrix(std::ifstream& TiledFile, TLayer& Out2DArray, int LayerH) const
{
	std::string Line;

	for (int y = 0; y < LayerH; y++)
	{
		std::getline(TiledFile, Line);
		Out2DArray.push_back(GetLayerSingleArray(Line));
	}
}

std::vector<int> Core::TilemapComponent::GetLayerSingleArray(const std::string& Line) const
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

void Core::TilemapComponent::GetLayerSize(const std::string& Line, int* OutW, int* OutH) const
{
	if (!OutW || !OutH) return;

	// Width
	std::string ValueStr = GetStringValueFromFlag(Line, "width=");
	*OutW = std::stoi(ValueStr);

	// Height
	ValueStr = GetStringValueFromFlag(Line, "height=");
	*OutH = std::stoi(ValueStr);
}

std::string Core::TilemapComponent::GetStringValueFromFlag(const std::string& Line, const std::string& Flag) const
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

std::string Core::TilemapComponent::GetDelimitedStringValue(const std::string& Line, const size_t Begin, const size_t End) const
{
	std::string WidthStr;
	for (size_t i = Begin; i < End; i++)
	{
		WidthStr += Line[i];
	}

	return WidthStr;
}

void Core::TilemapComponent::AddObjectLayer(const std::string& Name)
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
			ConstructRectsFromObjectLayer(TiledFile, Line);
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

void Core::TilemapComponent::GetObjectData(const std::string& Line, std::string& OutId, float* X, float* Y, float* W, float* H) const
{
	if (!X || !Y || !W || !H) return;

	OutId = GetStringValueFromFlag(Line, "id=");
	*X = (float)std::stof(GetStringValueFromFlag(Line, "x="));
	*Y = (float)std::stof(GetStringValueFromFlag(Line, "y="));
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

void Core::TilemapComponent::ConstructRectsFromObjectLayer(std::ifstream& TiledFile, const std::string& FirstLine)
{
	std::string Line = FirstLine;

	while (Line.find("</objectgroup>") == Line.npos)
	{
		Rect<float> ObjectRect;
		std::string ObjectId;
		GetObjectData(Line, ObjectId, &ObjectRect.X, &ObjectRect.Y, &ObjectRect.W, &ObjectRect.H);

		TilemapObject Obj(ObjectId, ObjectRect);
		mTilemapObjects->push_back(Obj);
		std::getline(TiledFile, Line);
	}
}

void Core::TilemapComponent::Draw()
{
	// draw each layer with his corresponding tilset, based on firstid and lastid of the tileset.
	for (auto& pair : *mTilemap)
	{
		TLayer Layer = pair.second._Layer;
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
				Rect<int> Dst(x * (int)mScaledTileSize.X, y * (int)mScaledTileSize.Y, (int)mScaledTileSize.X, (int)mScaledTileSize.Y);


				Graphic().DrawTexture(T._ImageId, Src, Dst, 0.0, Flip::None, Color::White);
			}
		}
	}
}

Core::Tileset Core::TilemapComponent::GetTilesetBasedOnTileId(int TileId) const
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

std::vector<Core::Tile> Core::TilemapComponent::GetListOfTileForLayer(const Layer& Layer)
{
	std::vector<Tile> OutList;
	for (int y = 0; y < Layer._Layer.size(); y++)
	{
		for (int x = 0; x < Layer._Layer[y].size(); x++)
		{
			int TileId = Layer._Layer[y][x];
			if (TileId == 0) continue;

			Tileset T = GetTilesetBasedOnTileId(TileId);
			TileId -= T._FirstId - 1;
			if (TileId >= T._Sources.size()) continue;

			Tile tile;
			tile.ParentLayerName = Layer._Name;
			tile.X = x * mScaledTileSize.X;
			tile.Y = y * mScaledTileSize.Y;
			tile.W = mScaledTileSize.X;
			tile.H = mScaledTileSize.Y;

			OutList.push_back(tile);
		}
	}
	return OutList;
}

std::vector<Core::TilemapObject> Core::TilemapComponent::GetTilemapObjects() const
{
	if (!mTilemapObjects) return std::vector<TilemapObject>();
	return *mTilemapObjects;
}

void Core::TilemapComponent::SetTiledFile(const std::string& File)
{
	mTiledFile = new std::string(File);
}

void Core::TilemapComponent::SetTilemapData(const std::string& TiledFile, float SrcTileW, float SrcTileH, int TileCountW, int TileCountH)
{
	mTiledFile = mTiledFile == nullptr ? new std::string(TiledFile) : mTiledFile;
	mTilesets = mTilesets == nullptr ? new std::vector<Tileset>() : mTilesets;
	mTilemap = mTilemap == nullptr ? new TTilemap() : mTilemap;
	mTileCount = Vector<int>(TileCountW, TileCountH);
	mTilemapObjects = mTilemapObjects == nullptr ? new std::vector<TilemapObject>() : mTilemapObjects;

	int WindowW, WindowH = 0;
	Graphic().GetWindowSize(&WindowW, &WindowH);
	mInitialTileSize.X = SrcTileW;
	mInitialTileSize.Y = SrcTileH;
	mScaledTileSize.X = (float)(WindowW / mTileCount.X);
	mScaledTileSize.Y = (float)(WindowH / mTileCount.Y);
}

void Core::TilemapComponent::Destroy()
{
	// récupérer chaque TLayer de la Tilemap
	// clear le 2d array TLayer

	// clear chaque tileset de mTilesets

	for (auto& Pair : *mTilemap)
	{
		TLayer L = Pair.second._Layer;
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

	mTilemapObjects->clear();

	delete mTiledFile;
	delete mTilesets;
	delete mTilemap;
	delete mTilemapObjects;
}

Core::TTilemap Core::TilemapComponent::GetTilemap() const
{
	if (mTilemap)
	{
		return *mTilemap;
	}
	return TTilemap();
}

std::vector<Core::Layer> Core::TilemapComponent::GetLayers() const
{
	if(!mTilemap) return std::vector<Layer>();

	std::vector<Layer> OutList;
	for (auto& pair : *mTilemap)
	{
		OutList.push_back(pair.second);
	}
	return OutList;
}
