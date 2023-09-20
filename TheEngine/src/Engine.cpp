#include "Engine.h"
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
	if (!GetLogger()->Init()) return false;

	GetInstance()->mGraphic = new SdlGraphic(Name, Width, Height);
	if (!GetGraphic()->Init(&InitMsg)) return false;

	GetInstance()->mInput = new SdlInput();

	GetInstance()->mTimer = new SdlTimer();
	GetTimer()->SetTargetFps(60.f);

	GetInstance()->mIsInit = true;
	GetLogger()->DebugLog(ConsoleColor::White, InitMsg);

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

	GetTimer()->StartTimer();

	Object* Obj1 = GetInstance()->GetWorld()->CreateObject<Object>();
	std::string Id = Obj1->GetId();
	GetLogger()->DebugLog(ConsoleColor::Cyan, "Id: %s", Id.c_str());

	while (GetInstance()->mIsRunning)
	{
		if (GetInput()->IsKeyDown(EKey::V))
		{
			GetLogger()->DebugLog(ConsoleColor::Red, "V was pressed");
		}
		Rect<int> R{200, 200, 512, 512};
	
		GetTimer()->UpdateStart();
		const float DeltaTime = GetInstance()->mTimer->GetDeltaTime();

		GetInstance()->ProcessInput();
		GetInstance()->Update(DeltaTime);
		GetInstance()->Render();

		GetTimer()->UpdateEnd();
	}

	GetInstance()->Shutdown();
}

void Core::Engine::ProcessInput(void)
{
	GetInput()->Update();
}

void Core::Engine::Update(float DeltaTime)
{
	if (GetInput()->ShouldQuit())
	{
		mIsRunning = false;
	}
}

void Core::Engine::Render(void)
{
	mGraphic->SetDrawColor(Color::Black);
	mGraphic->Clear();
	mGraphic->DrawRectF(false, 100.f, 100.f, 100.f, 100.f, Color::Red);
	mGraphic->DrawLineF(Vector2D<float>(300.f, 100.f), Vector2D<float>(375.f, 300.f), Color::Green);
	//size_t textureId = _Graphic->LoadTexture("../ArchHero/Assets/Helmet.png");
	mGraphic->Present();
}

void Core::Engine::Shutdown(void)
{
	if (!GetInstance()) return;
	delete GetInstance()->mInput;
	delete GetInstance()->mLogger;
	delete GetInstance()->mGraphic;
	delete GetInstance()->mTimer;
	delete GetInstance();
}