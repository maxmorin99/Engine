#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include "Engine.h"

using namespace Core;

void InitGameplay(void)
{

}

INT WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	if (Engine::Init("TestGame", 800, 600))
	{
		InitGameplay();
		Engine::Start();
	}

	return 0;
}