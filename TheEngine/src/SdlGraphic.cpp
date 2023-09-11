#include "SdlGraphic.h"
#include <SDL.h>

Core::SdlGraphic::SdlGraphic(const char* WinName, const int WinW, const int WinH)
{
	_WindowName = WinName;
	_WindowW = WinW;
	_WindowH = WinH;
}

bool Core::SdlGraphic::Init(const char* ErrorMsg)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		ErrorMsg = SDL_GetError();
		return false;
	}
	if(!InitWindow(ErrorMsg))
	{
		return false;
	}
	if(!InitRenderer(ErrorMsg))
	{
		return false;
	}
	return true;
}

bool Core::SdlGraphic::InitWindow(const char* ErrorMsg)
{
	const int X = SDL_WINDOWPOS_CENTERED;
	const int Y = SDL_WINDOWPOS_CENTERED;
	const Uint32 WindowFlags = SDL_WINDOW_TOOLTIP;

	_Window = SDL_CreateWindow(_WindowName, X, Y, _WindowW, _WindowH, WindowFlags);
	if (!_Window)
	{
		ErrorMsg = SDL_GetError();
		return false;
	}
	return true;
}

bool Core::SdlGraphic::InitRenderer(const char* ErrorMsg)
{
	const Uint32 RendererFlags = SDL_RENDERER_ACCELERATED;
	_Renderer = SDL_CreateRenderer(_Window, -1, RendererFlags);
	if (!_Renderer)
	{
		ErrorMsg = SDL_GetError();
		return false;
	}
	return true;
}

void Core::SdlGraphic::SetDrawColor(const int Red, const int Green, const int Blue, const int Alpha)
{
	SDL_SetRenderDrawColor(_Renderer, (Uint8)Red, (Uint8)Green, (Uint8)Blue, (Uint8)Alpha);
}

void Core::SdlGraphic::Clear()
{
	SDL_RenderClear(_Renderer);
}

void Core::SdlGraphic::Present()
{
	SDL_RenderPresent(_Renderer);
}

void Core::SdlGraphic::DrawRect(float RectX, float RectY, float RectW, float RectH)
{
	const SDL_FRect Rect{ RectX, RectY, RectW, RectH };
	SDL_RenderDrawRectF(_Renderer, &Rect);
}

void Core::SdlGraphic::ShutDown()
{
	SDL_DestroyRenderer(_Renderer);
	SDL_DestroyWindow(_Window);
	SDL_Quit();
}