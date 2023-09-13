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

void Core::SdlGraphic::SetDrawColor(const EColor NewColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(NewColor);
	SDL_SetRenderDrawColor(_Renderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
}

void Core::SdlGraphic::Clear()
{
	SDL_RenderClear(_Renderer);
}

void Core::SdlGraphic::Present()
{
	SDL_RenderPresent(_Renderer);
}

void Core::SdlGraphic::DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH, const EColor DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(_Renderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
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

void Core::SdlGraphic::DrawRect(bool bFill, Rect<int>* Rect, const EColor DrawColor)
{
	if (!Rect) return;
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(_Renderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	const SDL_Rect SdlRect{ Rect->X, Rect->Y, Rect->W, Rect->H };
	if (bFill)
	{
		SDL_RenderFillRect(_Renderer, &SdlRect);
	}
	else
	{
		SDL_RenderDrawRect(_Renderer, &SdlRect);
	}
}

void Core::SdlGraphic::DrawRectF(bool bFill, float RectX, float RectY, float RectW, float RectH, const EColor DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(_Renderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
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

void Core::SdlGraphic::DrawRectF(bool bFill, Rect<float>* Rect, const EColor DrawColor)
{
	if (!Rect) return;
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(_Renderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	const SDL_FRect SdlRect{ Rect->X, Rect->Y, Rect->W, Rect->H };
	if (bFill)
	{
		SDL_RenderFillRectF(_Renderer, &SdlRect);
	}
	else
	{
		SDL_RenderDrawRectF(_Renderer, &SdlRect);
	}
}

void Core::SdlGraphic::DrawLine(const float X1, const float Y1, const float X2, const float Y2, const EColor DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(_Renderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	SDL_RenderDrawLineF(_Renderer, X1, Y1, X2, Y2);
}

size_t Core::SdlGraphic::LoadTexture(const char* FileName)
{
	/*if (_Map.count(FileName) > 0)
	{
		return _Map.at(FileName);
	}
	SDL_Texture* Texture = IMG_LoadTexture(_Renderer, FileName);
	if (Texture)
	{
		_Map[FileName] = 0;
	}
	*/
	return size_t();
}

void Core::SdlGraphic::ShutDown()
{
	SDL_DestroyRenderer(_Renderer);
	SDL_DestroyWindow(_Window);
	SDL_Quit();
}

SDL_Color Core::SdlGraphic::ConvertToSdlColor(EColor InColor) const
{
	SDL_Color Color;
	switch (InColor)
	{
	case EColor::Black:
		Color.r = 0;
		Color.g = 0;
		Color.b = 0;
		break;
	case EColor::Blue:
		Color.r = 0;
		Color.g = 0;
		Color.b = 255;
		break;
	case EColor::Green:
		Color.r = 0;
		Color.g = 255;
		Color.b = 0;
		break;
	case EColor::Cyan:
		Color.r = 0;
		Color.g = 255;
		Color.b = 255;
		break;
	case EColor::Red:
		Color.r = 255;
		Color.g = 0;
		Color.b = 0;
		break;
	case EColor::Purple:
		Color.r = 255;
		Color.g = 0;
		Color.b = 255;
		break;
	case EColor::Yellow:
		Color.r = 255;
		Color.g = 255;
		Color.b = 0;
		break;
	case EColor::White:
		Color.r = 255;
		Color.g = 255;
		Color.b = 255;
		break;
	case EColor::Grey:
		Color.r = 128;
		Color.g = 128;
		Color.b = 128;
		break;
	default:
		break;
	}
	Color.a = 255;
	return Color;
}