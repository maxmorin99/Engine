#pragma once

namespace Core
{
	struct Color
	{
		Color(int R, int G, int B, int A) { Red = R, Green = G, Blue = B, Alpha = A; }
		int Red = 0;
		int Green = 0;
		int Blue = 0;
		int Alpha = 0;
	};

	template <class T>
	class Rect;

	class IGraphic
	{
	public:
		virtual ~IGraphic() = default;
		virtual bool Init(const char* ErrorMsg = nullptr) = 0;
		virtual bool InitWindow(const char* ErrorMsg = nullptr) = 0;
		virtual bool InitRenderer(const char* ErrorMsg = nullptr) = 0;
		virtual void SetDrawColor(const Color& NewColor) = 0;
		virtual void Clear() = 0;
		virtual void Present() = 0;
		virtual void DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH) = 0;
		virtual void DrawRect(bool bFill, Rect<int>* Rect) = 0;
		virtual void DrawRectF(bool bFill, const float RectX, const float RectY, const float RectW, const float RectH) = 0;
		virtual void DrawRectF(bool bFill, Rect<float>* Rect) = 0;
		virtual void DrawLine(const float X1, const float Y1, const float X2, const float Y2) = 0;
		virtual size_t LoadTexture(const char* Filename) = 0;
		//virtual void DrawTexture(size_t Id, const Rect<float>* Dst) = 0;
		virtual void ShutDown() = 0;
	};

}
