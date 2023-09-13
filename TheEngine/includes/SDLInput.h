#pragma once
#include "Interfaces/IInput.h"

namespace Core
{
	class SdlInput final : public IInput
	{
	public:
		SdlInput() = default;

		// Input interface
		virtual ~SdlInput() override = default;
		virtual void Update() override;
		virtual bool IsKeyDown(int Keycode) override;
		virtual bool IsButtonDown(int Button) override;
		virtual void GetMousePosition(int* X, int* Y) override;
		// End Input interface

		bool ShouldQuit() override;
		

	private:
		// Inputs
		const unsigned char* _KeyStates = nullptr;
		int _MouseX = 0;
		int _MouseY = 0;
		bool _MouseStates[3]{ false, false, false };

		bool _bQuit = false;
	};
}
