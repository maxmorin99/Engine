#include "Services/SdlGraphic.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>

Core::SdlGraphic::SdlGraphic(const char* WinName, const int WinW, const int WinH)
{
	mWindowName = WinName;
	mWindowWidth = WinW;
	mWindowHeight = WinH;
}

bool Core::SdlGraphic::Init(const char** ErrorMsg)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		*ErrorMsg = SDL_GetError();
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
	if (TTF_Init() != 0)
	{
		*ErrorMsg = SDL_GetError();
		return false;
	}
	*ErrorMsg = "Graphic init was succesful\n";
	return true;
}

bool Core::SdlGraphic::InitWindow(const char** ErrorMsg)
{
	const int X = SDL_WINDOWPOS_CENTERED;
	const int Y = SDL_WINDOWPOS_CENTERED;
	const Uint32 WindowFlags = SDL_WINDOW_TOOLTIP;

	mWindow = SDL_CreateWindow(mWindowName, X, Y, mWindowWidth, mWindowHeight, WindowFlags);
	if (!mWindow)
	{
		*ErrorMsg = SDL_GetError();
		return false;
	}
	return true;
}

bool Core::SdlGraphic::InitRenderer(const char** ErrorMsg)
{
	const Uint32 RendererFlags = SDL_RENDERER_ACCELERATED;
	mRenderer = SDL_CreateRenderer(mWindow, -1, RendererFlags);
	if (!mRenderer)
	{
		*ErrorMsg = SDL_GetError();
		return false;
	}
	return true;
}

void Core::SdlGraphic::SetDrawColor(const Color& NewColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(NewColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
}

void Core::SdlGraphic::Clear()
{
	SDL_RenderClear(mRenderer);
}

void Core::SdlGraphic::Present()
{
	SDL_RenderPresent(mRenderer);
}

void Core::SdlGraphic::DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH, const Color& DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	SDL_Rect Rect{ RectX, RectY, RectW, RectH };
	if (bFill)
	{
		SDL_RenderFillRect(mRenderer, &Rect);
	}
	else
	{
		SDL_RenderDrawRect(mRenderer, &Rect);
	}
}

void Core::SdlGraphic::DrawRect(bool bFill, Rect<int>* Rect, const Color& DrawColor)
{
	if (!Rect) return;
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	const SDL_Rect SdlRect{ Rect->X, Rect->Y, Rect->W, Rect->H };
	if (bFill)
	{
		SDL_RenderFillRect(mRenderer, &SdlRect);
	}
	else
	{
		SDL_RenderDrawRect(mRenderer, &SdlRect);
	}
}

void Core::SdlGraphic::DrawRectF(bool bFill, float RectX, float RectY, float RectW, float RectH, const Color& DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	const SDL_FRect Rect{ RectX, RectY, RectW, RectH };
	if (bFill)
	{
		SDL_RenderFillRectF(mRenderer, &Rect);
	}
	else
	{
		SDL_RenderDrawRectF(mRenderer, &Rect);
	}
}

void Core::SdlGraphic::DrawRectF(bool bFill, Rect<float>* Rect, const Color& DrawColor)
{
	if (!Rect) return;
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	const SDL_FRect SdlRect{ Rect->X, Rect->Y, Rect->W, Rect->H };
	if (bFill)
	{
		SDL_RenderFillRectF(mRenderer, &SdlRect);
	}
	else
	{
		SDL_RenderDrawRectF(mRenderer, &SdlRect);
	}
}

void Core::SdlGraphic::DrawLineF(const float X1, const float Y1, const float X2, const float Y2, const Color& DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	SDL_RenderDrawLineF(mRenderer, X1, Y1, X2, Y2);
}

void Core::SdlGraphic::DrawLine(const int X1, const int Y1, const int X2, const int Y2, const Color& DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	SDL_RenderDrawLine(mRenderer, X1, Y1, X2, Y2);
}

void Core::SdlGraphic::DrawLine(const Vector<int>& Point1, const Vector<int>& Point2, const Color& DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	SDL_RenderDrawLine(mRenderer, Point1.X, Point1.Y, Point2.X, Point2.Y);
}

void Core::SdlGraphic::DrawLineF(const Vector<float>& Point1, const Vector<float>& Point2, const Color& DrawColor)
{
	SDL_Color SdlColor = ConvertToSdlColor(DrawColor);
	SDL_SetRenderDrawColor(mRenderer, SdlColor.r, SdlColor.g, SdlColor.b, SdlColor.a);
	SDL_RenderDrawLineF(mRenderer, Point1.X, Point1.Y, Point2.X, Point2.Y);
}

size_t Core::SdlGraphic::LoadTexture(const char* FileName)
{
	size_t TextureId = std::hash<std::string>()(FileName);

	if (mTextureMap.find(TextureId) == mTextureMap.end())
	{
		SDL_Texture* Texture = IMG_LoadTexture(mRenderer, FileName);
		if (!Texture) return -1;
		mTextureMap[TextureId] = Texture;
	}
	
	return TextureId;
}

void Core::SdlGraphic::DrawTexture(size_t Id, const Rect<int>& Src, const Rect<int>& Dst, double Angle, const Flip& TextureFlip, const Color& Color)
{
	if (mTextureMap.find(Id) == mTextureMap.end()) return;
	SDL_Texture* Texture = mTextureMap[Id];
	if (!Texture) return;

	SDL_Rect SrcRect{ Src.X, Src.Y, Src.W, Src.H };
	SDL_Rect DstRect{ Dst.X, Dst.Y, Dst.W, Dst.H };

	SDL_RendererFlip Flip = SDL_FLIP_NONE;
	if (TextureFlip.H && !TextureFlip.V) // Flip horizontally
	{
		Flip = SDL_FLIP_HORIZONTAL;
	}
	else if (!TextureFlip.H && TextureFlip.V) // Flip vertically
	{
		Flip = SDL_FLIP_VERTICAL;
	}
	else if (TextureFlip.H && TextureFlip.V) // Flip horizontally and vertically
	{
		Flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	}
	
	SDL_Color SdlColor = ConvertToSdlColor(Color);
	SDL_SetTextureColorMod(Texture, SdlColor.r, SdlColor.g, SdlColor.b);
	SDL_SetTextureAlphaMod(Texture, (Uint8)Color.A);
	SDL_RenderCopyEx(mRenderer, Texture, &SrcRect, &DstRect, Angle, nullptr, Flip);
}

void Core::SdlGraphic::DrawTexture(size_t Id, const Rect<int>& Dst, const Color& Color)
{
	if (mTextureMap.find(Id) == mTextureMap.end()) return;
	SDL_Texture* Texture = mTextureMap[Id];
	if (!Texture) return;

	SDL_Rect DstRect{ Dst.X, Dst.Y, Dst.W, Dst.H };
	SDL_Color SdlColor = ConvertToSdlColor(Color);
	SDL_SetTextureColorMod(Texture, SdlColor.r, SdlColor.g, SdlColor.b);
	SDL_SetTextureAlphaMod(Texture, (Uint8)Color.A);
	SDL_RenderCopyEx(mRenderer, Texture, nullptr, &DstRect, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

void Core::SdlGraphic::GetTextureSize(size_t Id, int* OutW, int* OutH)
{
	if (mTextureMap.find(Id) == mTextureMap.end())
	{
		*OutW = 0;
		*OutH = 0;
		return;
	}
	SDL_Texture* Texture = mTextureMap[Id];
	if (!Texture)
	{
		*OutW = 0;
		*OutH = 0;
		return;
	}

	SDL_QueryTexture(Texture, nullptr, nullptr, OutW, OutH);
}

size_t Core::SdlGraphic::LoadFont(const char* FileName, int FontSize)
{
	size_t FontId = std::hash<std::string>()(FileName);
	if (mFontMap.find(FontId) == mFontMap.end())
	{
		TTF_Font* Font = TTF_OpenFont(FileName, FontSize);
		if (!Font) return -1;
		mFontMap[FontId] = Font;
	}
	return FontId;
}

void Core::SdlGraphic::DrawString(const char* Text, size_t FontId, float X, float Y, float W, float H, const Color& DrawColor)
{
	if (mFontMap.count(FontId) > 0)
	{
		SDL_Color StrColor = ConvertToSdlColor(DrawColor);
		TTF_Font* Font = mFontMap[FontId];
		SDL_Surface* Surface = TTF_RenderText_Solid(Font, Text, StrColor);
		SDL_Texture* TextureBuffer = SDL_CreateTextureFromSurface(mRenderer, Surface);
		SDL_Rect Dst = { static_cast<int>(X), static_cast<int>(Y), static_cast<int>(W), static_cast<int>(H) };
		SDL_RenderCopy(mRenderer, TextureBuffer, nullptr, &Dst);
		SDL_FreeSurface(Surface);
	}
}

void Core::SdlGraphic::GetTextSize(const char* Text, size_t FontId, int* OutW, int* OutH)
{
	if (mFontMap.count(FontId) > 0)
	{
		TTF_Font* Font = mFontMap[FontId];
		TTF_SizeText(Font, Text, OutW, OutH);
	}
}

void Core::SdlGraphic::ShutDown()
{
	// free textures and fonts
	for (auto& Pair : mTextureMap)
	{
		SDL_DestroyTexture(Pair.second);
	}
	for (auto& Pair : mFontMap)
	{
		TTF_CloseFont(Pair.second);
	}
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	TTF_Quit();
	SDL_Quit();
}

SDL_Color Core::SdlGraphic::ConvertToSdlColor(const Color& InColor) const
{
	SDL_Color Color;
	Color.r = InColor.R;
	Color.g = InColor.G;
	Color.b = InColor.B;
	Color.a = InColor.A;
	return Color;
}