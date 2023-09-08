#include "SDLInput.h"
#include <SDL.h>

void Core::SdlInput::Update()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
			_bQuit = true;
			break;
		default:
			break;
		}
	}

	_KeyStates = SDL_GetKeyboardState(nullptr);
}

bool Core::SdlInput::IsKeyDown(int Keycode)
{
	return _KeyStates[Keycode];
}

bool Core::SdlInput::IsButtonDown(int Button)
{
	return false;
}

void Core::SdlInput::GetMousePosition(int* x, int* y)
{
	*x = _MouseX;
	*y = _MouseY;
}

bool Core::SdlInput::ShouldQuit()
{
	return _bQuit;
}

