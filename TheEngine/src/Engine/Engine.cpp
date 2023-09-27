#include "Engine/Engine.h"
#include <ctime>
#include <Windows.h>
#include "Services/SdlGraphic.h"
#include "Services/SDLInput.h"
#include "Services/SdlTime.h"
#include "Services/World.h"
#include "Utility.h"
#if _DEBUG
#include "Services/SdlConsoleLogger.h"
#else
#include "SdlFileLogger.h"
#endif
#include "Object.h"

Core::Engine* Core::Engine::mInstance = nullptr;

Core::Engine* Core::Engine::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new Engine();
	}
	return mInstance;
}

bool Core::Engine::Init(const char* Name, int Width, int Height)
{
	const char* InitMsg = nullptr;

	if (GetInstance()->mIsInit) return false;

#if _DEBUG
	GetInstance()->mLogger = new SdlConsoleLogger();
#else
	GetInstance()->_Logger = new SdlFileLogger();
#endif
	if (!GetLogger().Init()) return false;

	GetInstance()->mGraphic = new SdlGraphic(Name, Width, Height);
	if (!GetGraphic().Init(&InitMsg)) return false;

	GetInstance()->mInput = new SdlInput();

	GetInstance()->mTimer = new SdlTimer();
	GetTimer().SetTargetFps(60.f);

	GetInstance()->mIsInit = true;
	GetLogger().DebugLog(ConsoleColor::White, InitMsg);

	GetInstance()->mWorld = new World();


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

	if (GetInstance()->mIsRunning) return;

	GetInstance()->mIsRunning = true;

	GetTimer().StartTimer();

	GetWorld().Start();

	while (GetInstance()->mIsRunning)
	{
		GetTimer().UpdateStart();
		const float DeltaTime = GetInstance()->mTimer->GetDeltaTime();

		ProcessInput();
		Update(DeltaTime);
		Render();

		GetTimer().UpdateEnd();
	}

	Shutdown();
}

void Core::Engine::ProcessInput(void)
{
	GetInput().Update();
}

void Core::Engine::Update(float DeltaTime)
{
	GetWorld().Update(DeltaTime);
	if (GetInput().ShouldQuit())
	{
		GetInstance()->mIsRunning = false;
	}
}

void Core::Engine::Render(void)
{
	GetGraphic().SetDrawColor(Color::Black);
	GetGraphic().Clear();
	//size_t textureId = _Graphic->LoadTexture("../ArchHero/Assets/Helmet.png");
	GetWorld().Render();
	GetGraphic().Present();
}

void Core::Engine::Shutdown(void)
{
	//EventManager::Shutdown();
	if (!GetInstance()) return;
	delete GetInstance()->mInput;
	delete GetInstance()->mLogger;
	delete GetInstance()->mGraphic;
	delete GetInstance()->mTimer;
	delete GetInstance();
	
}