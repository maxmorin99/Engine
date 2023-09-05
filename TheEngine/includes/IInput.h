#pragma once

class IInput
{
public:
	virtual ~IInput() = default;
	virtual bool IsKeyDown(int key) = 0;
	virtual bool IsButtonDown(int button) = 0;
	virtual void GetMousePosition(int* x, int* y) = 0;

protected:
	friend class Engine;
	virtual void Update() = 0;
};