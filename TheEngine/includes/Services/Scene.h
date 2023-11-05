#pragma once

#include "Interfaces/IScene.h"
#include <string>
#include <vector>

namespace Core
{
	class SdlTileMap;
	class Object;
	class CollisionComponent;
	
	class Scene : public IScene
	{
	public:
		Scene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH);
		virtual void Load() override;
		virtual void Destroy() override;

	private:
		std::string mName = "\0";

	protected:
		SdlTileMap* mTileMap = nullptr;
		std::vector<Object*> mObjectsToAddToWorld;
		std::vector<CollisionComponent*> mCollisionComponentsToAddToWorld;
	};
}