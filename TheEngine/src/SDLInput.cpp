#include "SDLInput.h"
#include <SDL.h>

void MyEngine::SdlInput::Update()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
			break;
		}
	}

	_KeyStates = SDL_GetKeyboardState(nullptr);
}

bool MyEngine::SdlInput::IsKeyDown(int keycode)
{
	return false;
}

bool MyEngine::SdlInput::IsButtonDown(int button)
{
	return false;
}

void MyEngine::SdlInput::GetMousePosition(int* x, int* y)
{
}
