#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include "Engine/Core.h"
#include "Engine/Engine.h"

using namespace Core;

void InitGameplay(void)
{
	IScene* S = new Scene("Scene01");
	Engine::GetWorld().Register("Scene01", S);
	Engine::GetWorld().Load("Scene01");
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