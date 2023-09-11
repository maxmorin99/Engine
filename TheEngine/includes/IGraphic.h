#pragma once

namespace Core
{
	class IGraphic
	{
	public:
		virtual ~IGraphic() = default;
		virtual bool Init(const char* ErrorMsg = nullptr) = 0;
		virtual bool InitWindow(const char* ErrorMsg = nullptr) = 0;
		virtual bool InitRenderer(const char* ErrorMsg = nullptr) = 0;
		virtual void SetDrawColor(const int Red, const int Green, const int Blue, const int Alpha) = 0;
		virtual void Clear() = 0;
		virtual void Present() = 0;
		virtual void DrawRect(const float RectX, const float RectY, const float RectW, const float RectH) = 0;
		virtual void ShutDown() = 0;
	};

}
