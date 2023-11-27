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
#include "Components/TargetCursorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TilemapComponent.h"
#include "Components/MusicComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/PathFindingComponent.h"
#include "Components/HealthBarComponent.h"

FirstScene::FirstScene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH) :
	Scene(name, tilemapFile, srcTileW, srcTileH, tileCountW, tileCountH)
{
	mTiledFile = tilemapFile;
}

void FirstScene::Load()
{
	Scene::Load();


	/* Tilemap ----------------------------------------------- */


	Object* TilemapObj = new Object();
	mObjectsToAddToWorld.push_back(TilemapObj);
	TilemapComponent* TmComp = TilemapObj->AddComponent<TilemapComponent>();
	TmComp->SetTilemapData(mTiledFile, (float)mSrcTileW, (float)mSrcTileH, mTileCountW, mTileCountH);

	std::string TilesetWall1 = ASSET_PATH + std::string("PrisonTileset/ERW - Old Prison V1.6.1/Tilesets/wall-1- 3 tiles tall.png");
	std::string TilesetWall2 = ASSET_PATH + std::string("PrisonTileset/ERW - Old Prison V1.6.1/Tilesets/wall-2- 3 tiles tall.png");
	std::string TilesetPrison = ASSET_PATH + std::string("PrisonTileset/ERW - Old Prison V1.6.1/Tilesets/Tileset-Terrain-old prison.png");
	TmComp->AddTileset(TilesetWall1, 1, 32, 32, 16, 368);
	TmComp->AddTileset(TilesetWall2, 369, 32, 32, 16, 240);
	TmComp->AddTileset(TilesetPrison, 609, 32, 32, 65, 4485);
	TmComp->AddLayer("BackgroundLayer");
	TmComp->AddLayer("FloorLayer");
	TmComp->AddLayer("WallLayer");
	TmComp->AddObjectLayer("CollisionLayer");

	std::vector<TilemapObject> TilemapObjects = TmComp->GetTilemapObjects();
	for (TilemapObject TilemapObj : TilemapObjects)
	{
		Object* Obj = new Object();
		mObjectsToAddToWorld.push_back(Obj);
		Obj->AddTag("World");
		Obj->SetLocation(TilemapObj._Rect.X, TilemapObj._Rect.Y);

		BoxComponent* Box = Obj->AddComponent<BoxComponent>();
		Box->SetCollisionChannel(ECollisionChannel::World);
		Box->SetBoxSize(TilemapObj._Rect.W, TilemapObj._Rect.H);
		Box->SetCollisionType(ECollisionShape::Rectangle);
		Box->SetCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Block);
		Box->SetCollisionResponseToChannel(ECollisionChannel::Enemy, ECollisionResponse::Block);
		Box->SetCollisionResponseToChannel(ECollisionChannel::Projectile, ECollisionResponse::Block);
		Box->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Ignore);
	}
	

	/* Init Animated Player ---------------------------------- */


	Object* AnimatedPlayer = new Object();
	mObjectsToAddToWorld.push_back(AnimatedPlayer);
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
	Anim->SetCenterOffset(Vector<float>(0, 50));

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
	mObjectsToAddToWorld.push_back(WeaponObj);
	WeaponObj->SetLocation(300, 300);
	WeaponObj->SetSize(60, 30);

	WeaponComponent* WeaponComp = WeaponObj->AddComponent<WeaponComponent>();
	std::string WeaponFile = ASSET_PATH + std::string("Weapons/testweapon.png");
	WeaponComp->SetFile(WeaponFile);
	WeaponComp->SetInstigator(AnimatedPlayer);
	WeaponComp->SetColor(Color::Black);
	WeaponComp->SetOffset(Vector<float>(30.f, 50.f));


	/* targetCursor -------------------------------------------- */


	Object* TargetCursorObj = new Object();
	mObjectsToAddToWorld.push_back(TargetCursorObj);
	TargetCursorObj->SetLocation(0.f, 0.f);
	TargetCursorObj->SetSize(25.f, 25.f);
	TargetCursorComponent* TargetCursorComp = TargetCursorObj->AddComponent<TargetCursorComponent>();
	std::string TargetCursorFile = ASSET_PATH + std::string("Weapons/crosshair.png");
	TargetCursorComp->SetFile(TargetCursorFile);
	TargetCursorComp->SetColor(Color::Black);

	WeaponObj->GetComponent<WeaponComponent>()->SetTargetCursorObject(TargetCursorObj);


	/* BoxComponent -------------------------------------------- */


	BoxComponent* Box = AnimatedPlayer->AddComponent<BoxComponent>();
	Box->SetCollisionChannel(ECollisionChannel::Player);
	Box->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::Enemy, ECollisionResponse::Ignore);
	Box->SetBoxSize(75, 100);
	Box->SetOffset(85, 120);


	/* Enemy -------------------------------------------- */
	
	Object* Enemy = new Object();
	Enemy->SetLocation(900, 450);
	Enemy->SetSize(250, 250);
	mObjectsToAddToWorld.push_back(Enemy);
	EnemyComponent* EnemyComp = Enemy->AddComponent<EnemyComponent>();
	PathFindingComponent* PathComp = Enemy->AddComponent<PathFindingComponent>();

	PhysicComponent* EnemyPxComp = Enemy->AddComponent<PhysicComponent>();
	EnemyPxComp->SetMass(1.f);
	EnemyPxComp->SetSlideFactor(0);
	EnemyPxComp->SetMaxMovementSpeed(100.f);
	EnemyPxComp->SetAccelerationSpeed(10000);
	EnemyPxComp->SetDecelerationSpeed(10000);

	// Animation
	AnimationComponent* EnemyAnimationComp = Enemy->AddComponent<AnimationComponent>();
	std::string EnemySpriteSheet = ASSET_PATH + std::string("Enemies/Enemy_1_Sprite_Sheet.png");
	EnemyAnimationComp->SetFile(EnemySpriteSheet);
	EnemyAnimationComp->SetCenterOffset(Vector<float>(0, 50));

	// Enemy1 Idle
	std::vector<Frame> Enemy1IdleFrames;
	Enemy1IdleFrames.push_back(Frame(0, 2, 2048, 2048, "Idle_1"));
	Enemy1IdleFrames.push_back(Frame(1, 2, 2048, 2048, "Idle_2"));
	Enemy1IdleFrames.push_back(Frame(2, 2, 2048, 2048, "Idle_3"));
	Enemy1IdleFrames.push_back(Frame(3, 2, 2048, 2048, "Idle_4"));
	Enemy1IdleFrames.push_back(Frame(4, 2, 2048, 2048, "Idle_5"));
	Enemy1IdleFrames.push_back(Frame(0, 3, 2048, 2048, "Idle_6"));
	Clip Enemy1IdleClip("Idle", Enemy1IdleFrames, 0.1f);
	EnemyAnimationComp->AddClip("Idle", Enemy1IdleClip);

	// Enemy1 Walk
	std::vector<Frame> Enemy1WalkFrames;
	Enemy1WalkFrames.push_back(Frame(1, 3, 2048, 2048, "Walk_1"));
	Enemy1WalkFrames.push_back(Frame(2, 3, 2048, 2048, "Walk_2"));
	Enemy1WalkFrames.push_back(Frame(3, 3, 2048, 2048, "Walk_3"));
	Enemy1WalkFrames.push_back(Frame(4, 3, 2048, 2048, "Walk_4"));
	Enemy1WalkFrames.push_back(Frame(0, 4, 2048, 2048, "Walk_5"));
	Enemy1WalkFrames.push_back(Frame(1, 4, 2048, 2048, "Walk_6"));
	Enemy1WalkFrames.push_back(Frame(2, 4, 2048, 2048, "Walk_7"));
	Enemy1WalkFrames.push_back(Frame(3, 4, 2048, 2048, "Walk_8"));
	Clip Enemy1WalkClip("Walk", Enemy1WalkFrames, 0.1f);
	EnemyAnimationComp->AddClip("Walk", Enemy1WalkClip);

	EnemyAnimationComp->SetDefaultClip(Enemy1IdleClip);
	EnemyAnimationComp->SetClip("Idle", true);

	// Enemy1 collision
	BoxComponent* EnemyBox = Enemy->AddComponent<BoxComponent>();
	EnemyBox->SetCollisionChannel(ECollisionChannel::Enemy);
	EnemyBox->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Ignore);
	EnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Ignore);
	EnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Enemy, ECollisionResponse::Ignore);
	EnemyBox->SetBoxSize(75, 100);
	EnemyBox->SetOffset(85, 120);


	/* Music Obj ----------------------------------------------- */

	Object* MusicObj = new Object();
	mObjectsToAddToWorld.push_back(MusicObj);
	MusicComponent* MusicComp = MusicObj->AddComponent<MusicComponent>();
	std::string MusicFile = ASSET_PATH + std::string("Music/MusicDefaultScene.wav");
	MusicComp->SetMusicFile(MusicFile);

	
	/* UI ------------------------------------------------------ */

	Object* UIObj = new Object();
	mObjectsToAddToWorld.push_back(UIObj);
	HealthBarComponent* HealthBarComp = UIObj->AddComponent<HealthBarComponent>();
	HealthBarComp->SetPaddingPercent(0.01f);
	HealthBarComp->SetSizeRatio(Vector<float>(0.2, 0.05));
	HealthBarComp->SetBorderSize(7);


	/* Add obj in the world ------------------------------------ */

	for (Object* Obj : mObjectsToAddToWorld)
	{
		Engine::GetWorld().AddObject(Obj);
	}
}
