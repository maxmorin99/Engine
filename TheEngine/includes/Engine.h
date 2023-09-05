#pragma once

#include <string>

class SDL_Renderer;
class SDL_Window;
class IInput;

class Engine final
{
public:
	bool Init(const char* Name, int Width, int Height);
	void Start(void);

private:
	void ProcessInput(void);
	void Update(float DeltaTime);
	void Render(void);
	void Shutdown(void);

private:
	bool _IsRunning = false;
	bool _IsInit = false;
	SDL_Renderer* _Renderer = NULL;
	SDL_Window* _Window = NULL;
	const uint8_t* _KeyStates = NULL;

	IInput* _Input;


public:
	inline IInput* Input() { return _Input; }
};