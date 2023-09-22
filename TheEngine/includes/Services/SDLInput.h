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
		virtual ~SdlInput() override = default;
		virtual void Update() override;
		virtual bool IsKeyDown(const EKey& Key) override;
		virtual bool IsButtonDown(int Button) override;
		virtual void GetMousePosition(int* X, int* Y) override;
		// End Input interface

		bool ShouldQuit() override;

	private:
		// Inputs
		const unsigned char* mKeyStates = nullptr;
		int mMouseX = 0;
		int mMouseY = 0;
		bool mMouseStates[3]{ false, false, false };
		bool mQuit = false;


		int GetSdlScancode(const EKey& Key) const;
	};
}
