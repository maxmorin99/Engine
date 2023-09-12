#include "SdlGraphic.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Rect.h"

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

void Core::SdlGraphic::SetDrawColor(const Color& NewColor)
{
	SDL_SetRenderDrawColor(_Renderer, (Uint8)NewColor.Red, (Uint8)NewColor.Green, (Uint8)NewColor.Blue, (Uint8)NewColor.Alpha);
}

void Core::SdlGraphic::Clear()
{
	SDL_RenderClear(_Renderer);
}

void Core::SdlGraphic::Present()
{
	SDL_RenderPresent(_Renderer);
}

void Core::SdlGraphic::DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH)
{
	SDL_Rect Rect{ RectX, RectY, RectW, RectH };
	if (bFill)
	{
		SDL_RenderFillRect(_Renderer, &Rect);
	}
	else
	{
		SDL_RenderDrawRect(_Renderer, &Rect);
	}
}

void Core::SdlGraphic::DrawRect(bool bFill, Rect<int>* Rect)
{
	int RectX = 0;
	int RectY = 0;
	int RectW = 0;
	int RectH = 0;
	Rect->GetPosition(&RectX, &RectY);
	Rect->GetSize(&RectW, &RectH);
	const SDL_Rect SdlRect{ RectX, RectY, RectW, RectH };
	if (bFill)
	{
		SDL_RenderFillRect(_Renderer, &SdlRect);
	}
	else
	{
		SDL_RenderDrawRect(_Renderer, &SdlRect);
	}
}

void Core::SdlGraphic::DrawRectF(bool bFill, float RectX, float RectY, float RectW, float RectH)
{
	const SDL_FRect Rect{ RectX, RectY, RectW, RectH };
	if (bFill)
	{
		SDL_RenderFillRectF(_Renderer, &Rect);
	}
	else
	{
		SDL_RenderDrawRectF(_Renderer, &Rect);
	}
}

void Core::SdlGraphic::DrawRectF(bool bFill, Rect<float>* Rect)
{
	float RectX = 0.f;
	float RectY = 0.f;
	float RectW = 0.f;
	float RectH = 0.f;
	Rect->GetPosition(&RectX, &RectY);
	Rect->GetSize(&RectW, &RectH);
	const SDL_FRect SdlRect{ RectX, RectY, RectW, RectH };
	if (bFill)
	{
		SDL_RenderFillRectF(_Renderer, &SdlRect);
	}
	else
	{
		SDL_RenderDrawRectF(_Renderer, &SdlRect);
	}
}

void Core::SdlGraphic::DrawLine(const float X1, const float Y1, const float X2, const float Y2)
{
	SDL_RenderDrawLineF(_Renderer, X1, Y1, X2, Y2);
}

size_t Core::SdlGraphic::LoadTexture(const char* Filename)
{
	return size_t();
}

void Core::SdlGraphic::ShutDown()
{
	SDL_DestroyRenderer(_Renderer);
	SDL_DestroyWindow(_Window);
	SDL_Quit();
}