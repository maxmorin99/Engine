#include "Engine/Engine.h"
#include <ctime>
#include <Windows.h>
#include "Services/SdlGraphic.h"
#include "Services/SdlInput.h"
#include "Services/SdlTime.h"
#include "Services/World.h"
#include "Services/SdlAudio.h"
#include "Services/Spawner.h"
#include "Utility.h"
#if _DEBUG
#include "Services/SdlConsoleLogger.h"
#else
#include "Services/SdlFileLogger.h"
#endif
#include "Object.h"


#define VLD 0

#if VLD
#include "vld.h"
#endif

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
#if VLD
	VLDEnable();
#endif

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

	// Spawner
	GetInstance()->mSpawner = new Spawner();

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

#if VLD
	VLDDisable();
#endif
}

void Core::Engine::QuitButtonPressed()
{
	GetInstance()->WindowQuitCallback();
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
	GetInstance()->GetSpawner().Shutdown();
	delete GetInstance()->mInput;
	delete GetInstance()->mLogger;
	delete GetInstance()->mAudio;
	delete GetInstance()->mGraphic;
	delete GetInstance()->mTimer;
	delete GetInstance()->mWorld;
	delete GetInstance()->mSpawner;
}