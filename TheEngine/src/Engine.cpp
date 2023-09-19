#include "Engine.h"
#include <ctime>
#include <Windows.h>
#include "SdlGraphic.h"
#include "SDLInput.h"
#include "SdlTime.h"
#include "World.h"
#include "Utility.h"
#if _DEBUG
#include "SdlConsoleLogger.h"
#else
#include "SdlFileLogger.h"
#endif
#include "Object.h"

Core::Engine* Core::Engine::_Instance = nullptr;

Core::Engine* Core::Engine::GetInstance()
{
	if (!_Instance)
	{
		_Instance = new Engine();
	}
	return _Instance;
}

bool Core::Engine::Init(const char* Name, int Width, int Height)
{
	const char* InitMsg = nullptr;

	if (GetInstance()->_IsInit) return false;

#if _DEBUG
	GetInstance()->_Logger = new SdlConsoleLogger();
#else
	GetInstance()->_Logger = new SdlFileLogger();
#endif
	if (!GetInstance()->_Logger->Init()) return false;

	GetInstance()->_Graphic = new SdlGraphic(Name, Width, Height);
	if (!GetInstance()->_Graphic->Init(&InitMsg)) return false;

	GetInstance()->_Input = new SdlInput();

	GetInstance()->_Timer = new SdlTimer();
	GetInstance()->_Timer->SetTargetFps(60.f);

	GetInstance()->_IsInit = true;
	GetInstance()->Logger()->DebugLog(ConsoleColor::White, InitMsg);

	GetInstance()->_World = new World();

	return true;
}

void Core::Engine::Start(void)
{
	if (!GetInstance()->_IsInit)
	{
		if (!Init("Unkown Title", 800, 600))
		{
			return;
		}
	}

	if (GetInstance()->_IsRunning) return;

	GetInstance()->_IsRunning = true;

	GetInstance()->_Timer->StartTimer();

	Object* Obj1 = GetInstance()->GetWorld()->CreateObject<Object>();
	size_t Id = Obj1->GetId();
	GetInstance()->Logger()->DebugLog(ConsoleColor::Cyan, "Id: %lli", Id);

	while (GetInstance()->_IsRunning)
	{
		Rect<int> R{200, 200, 512, 512};
	
		GetInstance()->_Timer->UpdateStart();
		const float DeltaTime = GetInstance()->_Timer->GetDeltaTime();

		GetInstance()->ProcessInput();
		GetInstance()->Update(DeltaTime);
		GetInstance()->Render();

		GetInstance()->_Timer->UpdateEnd();
	}

	GetInstance()->Shutdown();
}

void Core::Engine::ProcessInput(void)
{
	Input()->Update();
}

void Core::Engine::Update(float DeltaTime)
{
	if (Input()->ShouldQuit())
	{
		_IsRunning = false;
	}
}

void Core::Engine::Render(void)
{
	_Graphic->SetDrawColor(Color::Black);
	_Graphic->Clear();
	_Graphic->DrawRectF(false, 100.f, 100.f, 100.f, 100.f, Color::Red);
	_Graphic->DrawLineF(Vector2D<float>(300.f, 100.f), Vector2D<float>(375.f, 300.f), Color::Green);
	//size_t textureId = _Graphic->LoadTexture("../ArchHero/Assets/Helmet.png");
	_Graphic->Present();
}

void Core::Engine::Shutdown(void)
{
	if (!GetInstance()) return;
	delete GetInstance()->_Input;
	delete GetInstance()->_Logger;
	delete GetInstance()->_Graphic;
	delete GetInstance()->_Timer;
	delete GetInstance();
}