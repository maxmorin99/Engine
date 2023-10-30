#pragma once

#include "Utility.h"

namespace Core
{
	class IGraphic
	{
	public:
		virtual ~IGraphic() = default;
		virtual bool Init(const char** ErrorMsg = nullptr) = 0;
		virtual bool InitWindow(const char** ErrorMsg = nullptr) = 0;
		virtual bool InitRenderer(const char** ErrorMsg = nullptr) = 0;
		virtual void SetDrawColor(const Color& NewColor) = 0;
		virtual void Clear() = 0;
		virtual void Present() = 0;
		virtual void DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH, const Color& DrawColor) = 0;
		virtual void DrawRect(bool bFill, Rect<int>* Rect, const Color& DrawColor) = 0;
		virtual void DrawRectF(bool bFill, const float RectX, const float RectY, const float RectW, const float RectH, const Color& DrawColor) = 0;
		virtual void DrawRectF(bool bFill, Rect<float>* Rect, const Color& DrawColor) = 0;
		virtual void DrawLine(const int X1, const int Y1, const int X2, const int Y2, const Color& DrawColor) = 0;
		virtual void DrawLine(const Vector<int>& Point1, const Vector<int>& Point2, const Color& DrawColor) = 0;
		virtual void DrawLineF(const float X1, const float Y1, const float X2, const float Y2, const Color& DrawColor) = 0;
		virtual void DrawLineF(const Vector<float>& Point1, const Vector<float>& Point2, const Color& DrawColor) = 0;
		virtual size_t LoadTexture(const char* FileName) = 0;
		virtual void DrawTexture(size_t Id, const Rect<int>& Src, const Rect<int>& Dst, double Angle, const Flip& TextureFlip, const Color& Color) = 0;
		virtual void DrawTexture(size_t Id, const Rect<int>& Dst, const Color& Color) = 0;
		virtual void GetTextureSize(size_t Id, int* OutW, int* OutH) = 0;
		virtual size_t LoadFont(const char* FileName, int FontSize) = 0;
		virtual void DrawString(const char* Text, size_t FontId, float X, float Y, float W, float H, const Color& DrawColor) = 0;
		virtual void GetTextSize(const char* Text, size_t FontId, int* OutW, int* OutH) = 0;
		virtual void GetWindowSize(int* WinW, int* WinH) = 0;
		virtual void ShutDown() = 0;
	};

}
