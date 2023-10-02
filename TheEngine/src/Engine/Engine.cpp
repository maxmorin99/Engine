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
	const char* InitMsg = nullptr;

	if (GetInstance()->mIsInit) return false;

#if _DEBUG
	GetInstance()->mLogger = new SdlConsoleLogger();
#else
	GetInstance()->mLogger = new SdlFileLogger();
#endif
	if (!GetLogger().Init()) return false;

	GetInstance()->mGraphic = new SdlGraphic(Name, Width, Height);
	if (!GetGraphic().Init(&InitMsg)) return false;

	GetInstance()->mInput = new SdlInput();

	GetInstance()->mAudio = new SdlAudio();
	GetAudio().Init(&InitMsg);

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
	
#if _DEBUG
	// Bind WindowQuitCallback function to SDL_QUIT event
	GetInput().BindQuitFunction(std::bind(&Core::Engine::WindowQuitCallback, GetInstance()));
#endif
		
	if (GetInstance()->mIsRunning) return;
	GetInstance()->mIsRunning = true;

	GetTimer().StartTimer();
	GetWorld().Start();

	// Play music
	std::string File = ASSET_PATH + std::string("Music/Music.mp3");
	size_t MusicId = GetAudio().LoadMusic(File.c_str());
	GetAudio().PlayMusic(MusicId, true);
	GetAudio().SetMusicVolume(MusicId, 10.f);

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

	Shutdown();
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
	//EventManager::Shutdown();
	if (!GetInstance()) return;
	delete GetInstance()->mInput;
	delete GetInstance()->mLogger;
	delete GetInstance()->mGraphic;
	delete GetInstance()->mTimer;
	delete GetInstance();
	
}