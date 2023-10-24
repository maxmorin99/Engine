#include "FirstScene.h"
#include "Object.h"
#include "Engine/Engine.h"
#include "Interfaces/IWorld.h"
#include "Components/AtlasComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/Functionality.h"
#include "Components/AnimationComponent.h"
#include "Components/WeaponComponent.h"

FirstScene::FirstScene(const char* name) :
	Scene(name)
{
}

void FirstScene::Load()
{
	Scene::Load();

	/* Init Animated Player ---------------------------------- */

	Object* AnimatedPlayer = new Object();
	AnimatedPlayer->AddComponent<PlayerComponent>();
	AnimatedPlayer->SetLocation(200.f, 200.f);
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

	// Death animation
	std::vector<Frame> DeathFrames;
	DeathFrames.push_back(Frame(0, 0, 2048, 2048, "Death_1"));
	DeathFrames.push_back(Frame(1, 0, 2048, 2048, "Death_2"));
	DeathFrames.push_back(Frame(2, 0, 2048, 2048, "Death_3"));
	DeathFrames.push_back(Frame(3, 0, 2048, 2048, "Death_4"));
	DeathFrames.push_back(Frame(4, 0, 2048, 2048, "Death_5"));
	DeathFrames.push_back(Frame(5, 0, 2048, 2048, "Death_6"));
	DeathFrames.push_back(Frame(0, 1, 2048, 2048, "Death_7"));
	DeathFrames.push_back(Frame(1, 1, 2048, 2048, "Death_8"));
	DeathFrames.push_back(Frame(2, 1, 2048, 2048, "Death_9"));
	DeathFrames.push_back(Frame(3, 1, 2048, 2048, "Death_10"));
	Clip DeathClip = Clip("Death", DeathFrames, 0.065f);
	Anim->AddClip("Death", DeathClip);

	// Roll animation
	std::vector<Frame> RollFrames;
	RollFrames.push_back(Frame(4, 2, 2048, 2048, "Roll_1"));
	RollFrames.push_back(Frame(5, 2, 2048, 2048, "Roll_2"));
	RollFrames.push_back(Frame(0, 3, 2048, 2048, "Roll_3"));
	RollFrames.push_back(Frame(1, 3, 2048, 2048, "Roll_4"));
	RollFrames.push_back(Frame(2, 3, 2048, 2048, "Roll_5"));
	Clip RollClip = Clip("Roll", RollFrames, 0.065f);
	Anim->AddClip("Roll", RollClip);

	Anim->SetDefaultClip(IdleClip);
	Anim->SetClip("Idle", true);


	/* Weapon -------------------------------------------------- */

	Object* WeaponObj = new Object();
	WeaponObj->SetLocation(300, 300);
	WeaponObj->SetSize(60, 30);
	WeaponComponent* WeaponComp = WeaponObj->AddComponent<WeaponComponent>();
	std::string WeaponFile = ASSET_PATH + std::string("Weapons/testweapon.png");
	WeaponComp->SetFile(WeaponFile);
	WeaponComp->SetInstigator(AnimatedPlayer);
	WeaponComp->SetColor(Color::Black);



	/* Add obj in the world ------------------------------------ */

	Engine::GetWorld().AddObject(AnimatedPlayer);
	Engine::GetWorld().AddObject(WeaponObj);
	
}
