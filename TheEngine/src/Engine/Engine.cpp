#include "Engine/Engine.h"
#include <ctime>
#include <Windows.h>
#include "Services/SdlGraphic.h"
#include "Services/SdlInput.h"
#include "Services/SdlTime.h"
#include "Services/World.h"
#include "Services/SdlAudio.h"
#include "Utility.h"
#if _DEBUG
#include "Services/SdlConsoleLogger.h"
#else
#include "Services/SdlFileLogger.h"
#endif
#include "Object.h"
#include "vld.h"
#include "Services/SdlTileMap.h"

Core::Engine* Core::Engine::mInstance = nullptr;

Core::Engine* Core::Engine::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new Engine();
	}
	return mInstance;
}

void Core::Engine::WindowQuitCallback()
{
	mIsRunning = false;
}

bool Core::Engine::Init(const char* Name, int Width, int Height)
{
	//VLDEnable();

	const char* InitMsg = nullptr;

	if (GetInstance()->mIsInit) return false;

#if _DEBUG
	GetInstance()->mLogger = new SdlConsoleLogger();
#else
	GetInstance()->mLogger = new SdlFileLogger();
#endif
	if (!GetLogger().Init()) return false;

	// Graphics
	GetInstance()->mGraphic = new SdlGraphic(Name, Width, Height);
	if (!GetGraphic().Init(&InitMsg)) return false;

	// Tilemap
	std::string TiledFile = ASSET_PATH + std::string("PrisonTileset/test.tmx");
	GetInstance()->mTileMap = new SdlTileMap(TiledFile, 32, 32, 25, 19);

	std::string TilesetWall1 = ASSET_PATH + std::string("PrisonTileset/ERW - Old Prison V1.6.1/Tilesets/wall-1- 3 tiles tall.png");
	std::string TilesetWall2 = ASSET_PATH + std::string("PrisonTileset/ERW - Old Prison V1.6.1/Tilesets/wall-2- 3 tiles tall.png");
	std::string TilesetPrison = ASSET_PATH + std::string("PrisonTileset/ERW - Old Prison V1.6.1/Tilesets/Tileset-Terrain-old prison.png");
	GetInstance()->mTileMap->AddTileset(TilesetWall1, 1, 32, 32, 16, 368);
	GetInstance()->mTileMap->AddTileset(TilesetWall2, 369, 32, 32, 16, 240);
	GetInstance()->mTileMap->AddTileset(TilesetPrison, 609, 32, 32, 65, 4485);
	GetInstance()->mTileMap->AddLayer("FloorLayer");
	GetInstance()->mTileMap->AddLayer("BackgroundLayer");
	GetInstance()->mTileMap->AddObjectLayer("CollisionLayer");

	// Input
	GetInstance()->mInput = new SdlInput();

	// Audio
	GetInstance()->mAudio = new SdlAudio();
	GetAudio().Init(&InitMsg);

	// Timer
	GetInstance()->mTimer = new SdlTimer();
	GetTimer().SetTargetFps(60.f);

	GetInstance()->mIsInit = true;
	GetLogger().DebugLog(ConsoleColor::White, InitMsg);

	// World
	GetInstance()->mWorld = new World(*GetInstance()->mLogger);

	return true;
}

void Core::Engine::Start(void)
{
	if (!GetInstance()->mIsInit)
	{
		if (!Init("Unkown Title", 800, 600))
		{
			return;
		}
	}
	
#if _DEBUG
	// Bind WindowQuitCallback function to SDL_QUIT event
	GetInput().BindQuitFunction(std::bind(&Core::Engine::WindowQuitCallback, GetInstance()));
#endif
		
	if (GetInstance()->mIsRunning) return;
	GetInstance()->mIsRunning = true;

	GetTimer().StartTimer();
	GetWorld().Start();

	while (GetInstance()->mIsRunning)
	{
		// Quit if user press ESC key
		if (GetInput().IsKeyDown(EKey::ESC))
		{
			GetInstance()->mIsRunning = false;
		}

		GetTimer().UpdateStart();
		const float DeltaTime = GetInstance()->mTimer->GetDeltaTime();

		ProcessInput();
		Update(DeltaTime);
		Render();

		GetTimer().UpdateEnd();
	}

	GetInstance()->GetWorld().Unload();
	Update(0.f);
	Shutdown();

	delete mInstance;
	VLDDisable();
}

void Core::Engine::ProcessInput(void)
{
	GetInput().Update();
}

void Core::Engine::Update(float DeltaTime)
{
	GetWorld().Update(DeltaTime);
}

void Core::Engine::Render(void)
{
	GetGraphic().SetDrawColor(Color::Grey);
	GetGraphic().Clear();
	GetInstance()->mTileMap->Draw();
	GetWorld().Render();
	GetGraphic().Present();
}

void Core::Engine::Shutdown(void)
{
	if (!GetInstance()) return;
	GetInstance()->GetWorld().ShutDown();
	GetInstance()->GetLogger().Shutdown();
	GetInstance()->GetGraphic().ShutDown();
	GetInstance()->GetAudio().ShutDown();
	GetInstance()->mTileMap->Shutdown();
	delete GetInstance()->mInput;
	delete GetInstance()->mLogger;
	delete GetInstance()->mAudio;
	delete GetInstance()->mGraphic;
	delete GetInstance()->mTimer;
	delete GetInstance()->mWorld;
	delete GetInstance()->mTileMap;
}