#pragma once

#include "Interfaces/IGraphic.h"
#include <unordered_map>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Color;
struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;

namespace Core
{
	class SdlGraphic final : public IGraphic
	{
	public:
		SdlGraphic(const char* WinName, const int WinW, const int WinH);

		/** Graphic Interface */
		virtual ~SdlGraphic() override = default;
		virtual bool Init(const char** ErrorMsg = nullptr) override;
		virtual bool InitWindow(const char** ErrorMsg = nullptr) override;
		virtual bool InitRenderer(const char** ErrorMsg = nullptr) override;
		virtual void SetDrawColor(const Color& NewColor) override;
		virtual void Clear() override;
		virtual void Present() override;
		virtual void DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH, const Color& DrawColor) override;
		virtual void DrawRect(bool bFill, Rect<int>* Rect, const Color& DrawColor) override;
		virtual void DrawRectF(bool bFill, const float RectX, const float RectY, const float RectW, const float RectH, const Color& DrawColor) override;
		virtual void DrawRectF(bool bFill, Rect<float>* Rect, const Color& DrawColor) override;
		virtual void DrawLineF(const float X1, const float Y1, const float X2, const float Y2, const Color& DrawColor) override;
		virtual void DrawLine(const int X1, const int Y1, const int X2, const int Y2, const Color& DrawColor) override;
		virtual void DrawLine(const Vector<int>& Point1, const Vector<int>& Point2, const Color& DrawColor) override;
		virtual void DrawLineF(const Vector<float>& Point1, const Vector<float>& Point2, const Color& DrawColor) override;
		virtual void DrawPoint(const Vector<float>& Point, const Color& Color) override;
		virtual size_t LoadTexture(const char* FileName) override;
		virtual void DrawTexture(size_t Id, const Rect<int>& Src, const Rect<int>& Dst, double Angle, const Flip& TextureFlip, const Color& Color) override;
		virtual void DrawTexture(size_t Id, const Rect<int>& Dst, const Color& Color) override;
		virtual void GetTextureSize(size_t Id, int* OutW, int* OutH) override;
		virtual size_t LoadFont(const char* FileName, int FontSize) override;
		virtual void DrawString(const char* Text, size_t FontId, float X, float Y, float W, float H, const Color& DrawColor) override;
		virtual void GetTextSize(const char* Text, size_t FontId, int* OutW, int* OutH) override;
		virtual void ShutDown() override;
		virtual void GetWindowSize(int* WinW, int* WinH) override;
		/** End Graphic Interface */

	private:
		SDL_Window* mWindow = nullptr;
		SDL_Renderer* mRenderer = nullptr;
		const char* mWindowName = nullptr;
		int mWindowWidth = 0;
		int mWindowHeight = 0;

		std::unordered_map<size_t, SDL_Texture*> mTextureMap;
		std::unordered_map<size_t, TTF_Font*> mFontMap;

		SDL_Color ConvertToSdlColor(const Color& InColor) const;
	};
}