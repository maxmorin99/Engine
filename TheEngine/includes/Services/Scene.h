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
		std::string mTiledFile;
		int mSrcTileW = 0;
		int mSrcTileH = 0;
		int mTileCountW = 0;
		int mTileCountH = 0;

		/** When initializong a scene, this list contains all object created in this process to add to the world */
		std::vector<Object*> mObjectsToAddToWorld;

		/** When initializong a scene, this list contains all collision comp created in this process to add to the world */
		std::vector<CollisionComponent*> mCollisionComponentsToAddToWorld;
	};
}