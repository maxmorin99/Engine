#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include "Engine/Core.h"
#include "Engine/Engine.h"
#include <vector>
#include "DefaultScene.h"
#include "FirstScene.h"

using namespace Core;

void InitGameplay()
{
	// Init base scene
	/*IScene* S = new Scene("Scene01");
	Engine::GetWorld().Register("Scene01", S);
	Engine::GetWorld().Load("Scene01");*/

	IScene* Default = new DefaultScene("DefaultScene");
	IScene* First = new FirstScene("FirstScene");
	Engine::GetWorld().Register("DefaultScene", Default);
	Engine::GetWorld().Register("FirstScene", First);
	//Engine::GetWorld().Load("DefaultScene");
	Engine::GetWorld().Load("FirstScene");
}

INT WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	if (Engine::Init("TestGame", 1000, 850))
	{
		InitGameplay();
		Engine::Start();
	}

	return 0;
}