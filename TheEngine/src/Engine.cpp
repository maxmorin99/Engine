#include "Engine.h"
#include <SDL.h>
#include "SDLInput.h"

bool MyEngine::Engine::Init(const char* Name, int Width, int Height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log(SDL_GetError());
		return false;
	}

	int X = SDL_WINDOWPOS_CENTERED;
	int Y = SDL_WINDOWPOS_CENTERED;
	Uint32 WindowFlags = SDL_WINDOW_TOOLTIP;
	_Window = SDL_CreateWindow(Name, X, Y, Width, Height, WindowFlags);
	if (!_Window)
	{
		SDL_Log(SDL_GetError());
		return false;
	}

	Uint32 RendererFlags = SDL_RENDERER_ACCELERATED;
	_Renderer = SDL_CreateRenderer(_Window, -1, RendererFlags);
	if (!_Renderer)
	{
		SDL_Log(SDL_GetError());
		return false;
	}

	_IsInit = true;

	_Input = new SdlInput();
	
	return true;
}

void MyEngine::Engine::Start(void)
{
	if (!_IsInit)
	{
		if (!Init("Unkown Title", 800, 600))
		{
			return;
		}
	}

	_IsRunning = true;
	float TargetFps = 1000.f / 60.f;
	Uint32 End = SDL_GetTicks();

	while (_IsRunning)
	{
		Uint32 Start = SDL_GetTicks();
		float DeltaTime = (Start - End) * 0.001f;

		ProcessInput();
		Update(DeltaTime);
		Render();

		if (DeltaTime < TargetFps)
		{
			const float SleepTime = TargetFps - DeltaTime;
			SDL_Delay(SleepTime);
		}

		End = Start;
	}

	Shutdown();
}

void MyEngine::Engine::ProcessInput(void)
{
	Input()->Update();
}

void MyEngine::Engine::Update(float DeltaTime)
{
	if (_KeyStates[SDL_SCANCODE_D])
	{
	}
}

void MyEngine::Engine::Render(void)
{
	SDL_SetRenderDrawColor(_Renderer, 0, 0, 0, 255);
	SDL_RenderClear(_Renderer);

	SDL_SetRenderDrawColor(_Renderer, 255, 0, 0, 255);
	SDL_Rect Rect;

	Rect.x = 100;
	Rect.y = 100;
	Rect.w = 100;
	Rect.h = 100;

	SDL_RenderDrawRect(_Renderer, &Rect);
	SDL_RenderPresent(_Renderer);
}

void MyEngine::Engine::Shutdown(void)
{
	if (_Input)
	{
		delete _Input;
	}
	SDL_DestroyRenderer(_Renderer);
	SDL_DestroyWindow(_Window);
	SDL_Quit();
}
