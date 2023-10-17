#pragma once

#include "Interfaces/IInput.h"

namespace Core
{
	/*typedef void (*func_t)(void*, int yolo);
	static func_t func = NULL;*/

	class SdlInput final : public IInput
	{
	public:
		SdlInput();

		// Input interface
		virtual ~SdlInput() override;
		virtual void Update() override;
		virtual bool IsKeyDown(const EKey& Key) override;
		virtual bool IsButtonDown(int Button) override;
		virtual void GetMousePosition(int* X, int* Y) override;
		virtual void BindQuitFunction(std::function<void()> FunPtr) override;

	private:
		const unsigned char* mKeyStates = nullptr;
		int mMouseX = 0;
		int mMouseY = 0;
		int mMouseStates[3]{ 0, 0, 0 };
		std::function<void()> EngineQuitFunction;

		int GetSdlScancode(const EKey& Key) const;
	};
}
