#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include "Engine/Engine.h"
#include "Engine/Core.h"
#include "GameObject.h"

using namespace Core;

void InitGameplay(void)
{
	GameObject* Obj = Engine::GetWorld()->CreateObject<GameObject>();
	GameObject* Obj2 = Engine::GetWorld()->CreateObject<GameObject>();
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