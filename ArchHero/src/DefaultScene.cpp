#include "DefaultScene.h"
#include "Object.h"
#include "Engine/Engine.h"
#include "Interfaces/IWorld.h"
#include "Components/PlayerComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/Functionality.h"
#include "Components/PortalComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AnimationComponent.h"

DefaultScene::DefaultScene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH) :
	Scene(name, tilemapFile, srcTileW, srcTileH, tileCountW, tileCountH)
{
}

void DefaultScene::Load()
{
	Scene::Load();

	Object* Door = new Object();
	Door->SetLocation(700, 500);
	Door->SetSize(150, 150);
	Door->AddComponent<PortalComponent>();
	BoxComponent* DoorBox = Door->AddComponent<BoxComponent>();
	DoorBox->SetCollisionChannel(ECollisionChannel::World);
	DoorBox->AddCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Overlap);
	DoorBox->SetBoxSize(150, 150);
	DoorBox->SetOffset(0, 0);
	SpriteComponent* DoorSprite = Door->AddComponent<SpriteComponent>();
	std::string DoorFile = ASSET_PATH + std::string("Door/Door.png");
	DoorSprite->SetFile(DoorFile);
	

	/* Init Functionality obj ---------------------------------- */

	Object* FunctionalityObj = new Object();
	FunctionalityComponent* Functionality = FunctionalityObj->AddComponent<FunctionalityComponent>();
	std::string FontFile = ASSET_PATH + std::string("Font/Asset.ttf");
	std::string MusicFile = ASSET_PATH + std::string("Music/Music.mp3");
	Functionality->SetFontFile(FontFile.c_str(), 24);
	Functionality->SetMusicFile(MusicFile.c_str(), 100);


	/* Init Player --------------------------------------------- */

	Object* AnimatedPlayer = new Object();
	AnimatedPlayer->AddTag("Player");
	AnimatedPlayer->AddComponent<PlayerComponent>();
	AnimatedPlayer->SetLocation(250.f, 250.f);
	AnimatedPlayer->SetSize(250, 250);

	// Physic
	PhysicComponent* Px = AnimatedPlayer->AddComponent<PhysicComponent>();
	Px->SetMass(1.f);
	Px->SetSlideFactor(0);
	Px->SetMaxMovementSpeed(250.f);
	Px->SetAccelerationSpeed(10000);
	Px->SetDecelerationSpeed(10000);

	// Animation
	AnimationComponent* Anim = AnimatedPlayer->AddComponent<AnimationComponent>();
	std::string FilePath = ASSET_PATH + std::string("Character_Sprite_Sheet.png");
	Anim->SetFile(FilePath);
	Anim->SetCenterOffset(Vector<float>(0, 0));

	// Idle animation
	std::vector<Frame> IdleFrames;
	IdleFrames.push_back(Frame(4, 1, 2048, 2048, "Idle_1"));
	IdleFrames.push_back(Frame(5, 1, 2048, 2048, "Idle_2"));
	IdleFrames.push_back(Frame(0, 2, 2048, 2048, "Idle_3"));
	IdleFrames.push_back(Frame(1, 2, 2048, 2048, "Idle_4"));
	IdleFrames.push_back(Frame(2, 2, 2048, 2048, "Idle_5"));
	IdleFrames.push_back(Frame(3, 2, 2048, 2048, "Idle_6"));
	Clip IdleClip = Clip("Idle", IdleFrames, 0.1f);
	Anim->AddClip("Idle", IdleClip);

	// walk animation
	std::vector<Frame> WalkFrames;
	WalkFrames.push_back(Frame(3, 3, 2048, 2048, "Walk_1"));
	WalkFrames.push_back(Frame(4, 3, 2048, 2048, "Walk_2"));
	WalkFrames.push_back(Frame(5, 3, 2048, 2048, "Walk_3"));
	WalkFrames.push_back(Frame(0, 4, 2048, 2048, "Walk_4"));
	WalkFrames.push_back(Frame(1, 4, 2048, 2048, "Walk_5"));
	WalkFrames.push_back(Frame(2, 4, 2048, 2048, "Walk_6"));
	WalkFrames.push_back(Frame(3, 4, 2048, 2048, "Walk_7"));
	WalkFrames.push_back(Frame(4, 4, 2048, 2048, "Walk_8"));
	Clip WalkClip = Clip("Walk", WalkFrames, 0.05f);
	Anim->AddClip("Walk", WalkClip);

	/* BoxComponent -------------------------------------------- */

	BoxComponent* Box = AnimatedPlayer->AddComponent<BoxComponent>();
	Box->SetCollisionChannel(ECollisionChannel::Player);
	Box->AddCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Block);
	Box->SetBoxSize(75, 100);
	Box->SetOffset(85, 120);


	/* Add obj in the world ------------------------------------ */

	Engine::GetWorld().AddObject(FunctionalityObj);
	Engine::GetWorld().AddObject(Door);
	Engine::GetWorld().AddObject(AnimatedPlayer);
}
