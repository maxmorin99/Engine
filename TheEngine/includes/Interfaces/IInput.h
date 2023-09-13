#pragma once

namespace Core
{
	class IInput
	{
	public:
		virtual ~IInput() = default;
		virtual bool IsKeyDown(int Key) = 0;
		virtual bool IsButtonDown(int Button) = 0;
		virtual void GetMousePosition(int* X, int* Y) = 0;
		virtual bool ShouldQuit() = 0;

	protected:
		friend class Engine;
		virtual void Update() = 0;
	};
}
