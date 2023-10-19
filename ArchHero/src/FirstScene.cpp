#include "FirstScene.h"
#include "Object.h"
#include "Engine/Engine.h"
#include "Interfaces/IWorld.h"
#include "Components/AtlasComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/Functionality.h"

FirstScene::FirstScene(const char* name) :
	Scene(name)
{
}

void FirstScene::Load()
{
	Scene::Load();

	/* Init Functionality obj ---------------------------------- */

	Object* FunctionalityObj = new Object();
	FunctionalityComponent* Functionality = FunctionalityObj->AddComponent<FunctionalityComponent>();
	std::string FontFile = ASSET_PATH + std::string("Font/Pacifico.ttf");
	std::string MusicFile = ASSET_PATH + std::string("Music/BatCountry.mp3");
	std::string SoundFile = ASSET_PATH + std::string("Sfx/IceImpact.wav");
	Functionality->SetFontFile(FontFile.c_str(), 24);
	Functionality->SetMusicFile(MusicFile.c_str(), 100);


	/* Init Player --------------------------------------------- */

	Object* Player = new Object();
	Player->SetLocation(100.f, 100.f);
	Player->SetSize(250, 250);
	Player->AddComponent<PlayerComponent>();

	AtlasComponent* Atlas = Player->AddComponent<AtlasComponent>();
	std::string FilePath = ASSET_PATH + std::string("Character_Atlas.png");
	Atlas->SetFile(FilePath);
	Atlas->AddFrame("Head", 0, 5, 2048, 2048);
	Atlas->SetFrame("Head");

	PhysicComponent* Px = Player->AddComponent<PhysicComponent>();
	Px->SetMass(1.f);
	Px->SetSlideFactor(0);
	Px->SetMaxMovementSpeed(250.f);
	Px->SetAccelerationSpeed(10000);
	Px->SetDecelerationSpeed(10000);


	/* Add obj in the world ------------------------------------ */

	Engine::GetWorld().AddObject(FunctionalityObj);
	Engine::GetWorld().AddObject(Player);
}
