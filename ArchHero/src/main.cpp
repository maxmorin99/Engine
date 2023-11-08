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
	std::string TiledFile = ASSET_PATH + std::string("PrisonTileset/test.tmx");
	IScene* Default = new DefaultScene("DefaultScene", TiledFile.c_str(), 32, 32, 25, 19);
	IScene* First = new FirstScene("FirstScene", TiledFile.c_str(), 32, 32, 25, 19);
	Engine::GetWorld().Register("DefaultScene", Default);
	Engine::GetWorld().Register("FirstScene", First);
	//Engine::GetWorld().Load("DefaultScene");
	Engine::GetWorld().Load("FirstScene");
}

INT WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	if (Engine::Init("TestGame", 1200, 1200))
	{
		InitGameplay();
		Engine::Start();
	}

	return 0;
}