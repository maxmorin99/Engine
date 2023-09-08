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
			break;
		default:
			break;
		}
	}

	_KeyStates = SDL_GetKeyboardState(nullptr);
}

bool Core::SdlInput::IsKeyDown(int keycode)
{
	return false;
}

bool Core::SdlInput::IsButtonDown(int button)
{
	return false;
}

void Core::SdlInput::GetMousePosition(int* x, int* y)
{
	*x = _MouseX;
	*y = _MouseY;
}
