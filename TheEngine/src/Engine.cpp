#include "Engine.h"
#include <ctime>
#include <Windows.h>
#include "SdlGraphic.h"
#include "SDLInput.h"
#include "SdlTime.h"
#if _DEBUG
#include "SdlConsoleLogger.h"
#else
#include "SdlFileLogger.h"
#endif

bool Core::Engine::Init(const char* Name, int Width, int Height)
{
	// Init logger
#if _DEBUG
	_Logger = new SdlConsoleLogger();
#else
	_Logger = new SdlFileLogger();
#endif
	if (!_Logger->Init()) return false;

	// Init Graphic
	_Graphic = new SdlGraphic(Name, Width, Height);
	if (!_Graphic->Init()) return false;

	// Init Input
	_Input = new SdlInput();

	// Init Timer
	_Timer = new SdlTimer();
	_Timer->SetTargetFps(1.f);

	_IsInit = true;

	return true;
}

void Core::Engine::Start(void)
{
	if (!_IsInit)
	{
		if (!Init("Unkown Title", 800, 600))
		{
			return;
		}
	}

	_IsRunning = true;

	_Timer->StartTimer();

	while (_IsRunning)
	{
		_Timer->UpdateStart();

		const float DeltaTime = _Timer->GetDeltaTime();
		_Logger->DebugLog(EColor::Red, "DT: %f\n", DeltaTime);

		ProcessInput();
		Update(DeltaTime);
		Render();

		_Timer->UpdateEnd();
	}

	Shutdown();
}

void Core::Engine::ProcessInput(void)
{
	GetInput()->Update();
}

void Core::Engine::Update(float DeltaTime)
{
	if (GetInput()->ShouldQuit())
	{
		_IsRunning = false;
	}
}

void Core::Engine::Render(void)
{
	_Graphic->SetDrawColor(Color(255, 0, 0, 255));
	_Graphic->Clear();
	_Graphic->SetDrawColor(Color(255, 0, 0, 255));
	_Graphic->DrawRectF(false, 100.f, 100.f, 100.f, 100.f);
	_Graphic->Present();
}

void Core::Engine::Shutdown(void)
{
	delete _Input;
	delete _Logger;
	delete _Graphic;
	delete _Timer;
}
