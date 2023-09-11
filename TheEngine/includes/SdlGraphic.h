#pragma once

#include "IGraphic.h"

struct SDL_Window;
struct SDL_Renderer;

namespace Core
{
	class SdlGraphic : public IGraphic
	{
	public:
		SdlGraphic(const char* WinName, const int WinW, const int WinH);

		/** Graphic Interface */
		virtual ~SdlGraphic() override = default;
		virtual bool Init(const char* ErrorMsg = nullptr) override;
		virtual bool InitWindow(const char* ErrorMsg = nullptr) override;
		virtual bool InitRenderer(const char* ErrorMsg = nullptr) override;
		virtual void SetDrawColor(const int Red, const int Green, const int Blue, const int Alpha) override;
		virtual void Clear() override;
		virtual void Present() override;
		virtual void DrawRect(const float RectX, const float RectY, const float RectW, const float RectH) override;
		virtual void ShutDown() override;
		/** End Graphic Interface */

	private:
		SDL_Window* _Window = nullptr;
		SDL_Renderer* _Renderer = nullptr;
		const char* _WindowName = nullptr;
		int _WindowW = 0;
		int _WindowH = 0;
	};
}