#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include "Engine.h"

void InitGameplay(void)
{

}

INT WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	Core::Engine TheEngine;
	if (TheEngine.Init("TestGame", 800, 600))
	{
		InitGameplay();
		TheEngine.Start();
	}

	return 0;
}