#pragma once

#include "Interfaces/IGraphic.h"
#include <unordered_map>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Color;

namespace Core
{
	class SdlGraphic final : public IGraphic
	{
	public:
		SdlGraphic(const char* WinName, const int WinW, const int WinH);

		/** Graphic Interface */
		virtual ~SdlGraphic() override = default;
		virtual bool Init(const char* ErrorMsg = nullptr) override;
		virtual bool InitWindow(const char* ErrorMsg = nullptr) override;
		virtual bool InitRenderer(const char* ErrorMsg = nullptr) override;
		virtual void SetDrawColor(const EColor NewColor) override;
		virtual void Clear() override;
		virtual void Present() override;
		virtual void DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH, const EColor DrawColor) override;
		virtual void DrawRect(bool bFill, Rect<int>* Rect, const EColor DrawColor) override;
		virtual void DrawRectF(bool bFill, const float RectX, const float RectY, const float RectW, const float RectH, const EColor DrawColor) override;
		virtual void DrawRectF(bool bFill, Rect<float>* Rect, const EColor DrawColor) override;
		virtual void DrawLine(const float X1, const float Y1, const float X2, const float Y2, const EColor DrawColor) override;
		virtual size_t LoadTexture(const char* FileName) override;
		virtual void ShutDown() override;
		/** End Graphic Interface */

	private:
		SDL_Window* _Window = nullptr;
		SDL_Renderer* _Renderer = nullptr;
		const char* _WindowName = nullptr;
		int _WindowW = 0;
		int _WindowH = 0;

		std::unordered_map<const char*, size_t> _Map;
		SDL_Color ConvertToSdlColor(EColor InColor) const;
	};
}