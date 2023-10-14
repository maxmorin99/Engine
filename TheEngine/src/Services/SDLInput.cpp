#include "Services/SDLInput.h"
#include <SDL.h>

Core::SdlInput::SdlInput()
{
}

Core::SdlInput::~SdlInput()
{
    mKeyStates = nullptr;
}

void Core::SdlInput::Update()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
        case SDL_QUIT:
            EngineQuitFunction();
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&mMouseX, &mMouseY);
            break;
        case SDL_MOUSEBUTTONDOWN:
            mMouseStates[0] = SDL_BUTTON_LEFT;
            mMouseStates[1] = SDL_BUTTON_MIDDLE;
            mMouseStates[2] = SDL_BUTTON_RIGHT;
        case SDL_MOUSEBUTTONUP:
            mMouseStates[0] = SDL_BUTTON_LEFT;
            mMouseStates[1] = SDL_BUTTON_MIDDLE;
            mMouseStates[2] = SDL_BUTTON_RIGHT;
            break;
		}
	}
	mKeyStates = SDL_GetKeyboardState(nullptr);
}

bool Core::SdlInput::IsKeyDown(const EKey& Key)
{
    if (!mKeyStates) return false;

    int SdlCode = GetSdlScancode(Key);
	return mKeyStates[SdlCode];
}

bool Core::SdlInput::IsButtonDown(int Button)
{
	return false;
}

void Core::SdlInput::GetMousePosition(int* x, int* y)
{
	*x = mMouseX;
	*y = mMouseY;
}

void Core::SdlInput::BindQuitFunction(std::function<void()> FunPtr)
{
    EngineQuitFunction = FunPtr;
}

int Core::SdlInput::GetSdlScancode(const EKey& Key) const
{
    switch (Key) 
    {
    case EKey::A: return 4;
    case EKey::B: return 5;
    case EKey::C: return 6;
    case EKey::D: return 7;
    case EKey::E: return 8;
    case EKey::F: return 9;
    case EKey::G: return 10;
    case EKey::H: return 11;
    case EKey::I: return 12;
    case EKey::J: return 13;
    case EKey::K: return 14;
    case EKey::L: return 15;
    case EKey::M: return 16;
    case EKey::N: return 17;
    case EKey::O: return 18;
    case EKey::P: return 19;
    case EKey::Q: return 20;
    case EKey::R: return 21;
    case EKey::S: return 22;
    case EKey::T: return 23;
    case EKey::U: return 24;
    case EKey::V: return 25;
    case EKey::W: return 26;
    case EKey::X: return 27;
    case EKey::Y: return 28;
    case EKey::Z: return 29;
    case EKey::ESC: return 41;
    case EKey::Space: return 44;
    case EKey::Right: return 79;
    case EKey::Left: return 80;
    case EKey::Down: return 81;
    case EKey::Up: return 82;

    default: return -1;
    }
}

