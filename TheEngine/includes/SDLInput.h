#pragma once
#include "IInput.h"

namespace Core
{
	class SdlInput final : public IInput
	{
	public:
		// IInput interface
		virtual ~SdlInput() override = default;
		virtual void Update() override;
		virtual bool IsKeyDown(int Keycode) override;
		virtual bool IsButtonDown(int Button) override;
		virtual void GetMousePosition(int* X, int* Y) override;
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
