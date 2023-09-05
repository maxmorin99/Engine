#pragma once
#include "IInput.h"

class SdlInput final : public IInput
{
public:
	virtual ~SdlInput() = default;
	virtual void Update() override;
	virtual bool IsKeyDown(int keycode) override;
	virtual bool IsButtonDown(int button) override;
	virtual void GetMousePosition(int* x, int* y) override;
private:
	const unsigned char* _KeyStates = nullptr;
	int _MouseX = 0;
	int _MouseY = 0;
	bool _MouseStates[3]{ false, false, false };
};
