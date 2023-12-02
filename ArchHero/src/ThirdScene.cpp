#include "ThirdScene.h"
#include "Object.h"
#include "Engine/Engine.h"
#include "Interfaces/IWorld.h"
#include "Components/AtlasComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/TargetCursorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TilemapComponent.h"
#include "Components/MusicComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/PathFindingComponent.h"
#include "Components/HealthBarComponent.h"
#include "Components/ButtonComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/GateComponent.h"

ThirdScene::ThirdScene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH) :
	Scene(name, tilemapFile, srcTileW, srcTileH, tileCountW, tileCountH)
{
	mTiledFile = tilemapFile;
}

void ThirdScene::Load()
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
	TmComp->AddLayer("WallLayer");
	TmComp->AddLayer("FloorLayer");
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

	// Player
	Vector<float> PlayerTileStart(1, 2);
	Vector<float> PlayerStart = Vector<float>(PlayerTileStart.X * TmComp->GetTileSize().X, PlayerTileStart.Y * TmComp->GetTileSize().Y);
	Object* Player = Engine::GetSpawner().Spawn("Player");
	Player->SetLocation(PlayerStart);
	Player->SetSize(TmComp->GetTileSize().Y * 2.5f, TmComp->GetTileSize().Y * 2.5f);
	mObjectsToAddToWorld.push_back(Player);

	// Cursor
	Object* Cursor = Engine::GetSpawner().Spawn("Cursor");

	// Weapon
	Object* Weapon = Engine::GetSpawner().Spawn("Weapon");
	Weapon->SetSize(Player->GetSize().X * 0.24f, Player->GetSize().Y * 0.12f);
	Weapon->GetComponent<WeaponComponent>()->SetOffset(Vector<float>(Player->GetSize().X * 0.12f, Player->GetSize().Y * 0.2f));
	Weapon->GetComponent<WeaponComponent>()->SetInstigator(Player);
	Weapon->GetComponent<WeaponComponent>()->SetTargetCursorObject(Cursor);
	mObjectsToAddToWorld.push_back(Weapon);

	// Purple enemy 1
	Vector<float> PurpleEnemyTileStart(12, 4);
	Vector<float> PurpleEnemyStart = Vector<float>(PurpleEnemyTileStart.X * TmComp->GetTileSize().X, PurpleEnemyTileStart.Y * TmComp->GetTileSize().Y);
	Object* PurpleEnemyObj = Engine::GetSpawner().Spawn("PurpleEnemy");
	PurpleEnemyObj->SetLocation(PurpleEnemyStart);
	PurpleEnemyObj->SetSize(TmComp->GetTileSize().Y * 2.5f, TmComp->GetTileSize().Y * 2.5f);
	mObjectsToAddToWorld.push_back(PurpleEnemyObj);

	// Purple enemy 2
	Vector<float> PurpleEnemyTileStart2(7, 7);
	Vector<float> PurpleEnemyStart2 = Vector<float>(PurpleEnemyTileStart2.X * TmComp->GetTileSize().X, PurpleEnemyTileStart2.Y * TmComp->GetTileSize().Y);
	Object* PurpleEnemyObj2 = Engine::GetSpawner().Spawn("PurpleEnemy");
	PurpleEnemyObj2->SetLocation(PurpleEnemyStart2);
	PurpleEnemyObj2->SetSize(TmComp->GetTileSize().Y * 2.5f, TmComp->GetTileSize().Y * 2.5f);
	mObjectsToAddToWorld.push_back(PurpleEnemyObj2);

	// Purple enemy 3
	Vector<float> PurpleEnemyTileStart3(17, 7);
	Vector<float> PurpleEnemyStart3 = Vector<float>(PurpleEnemyTileStart3.X * TmComp->GetTileSize().X, PurpleEnemyTileStart3.Y * TmComp->GetTileSize().Y);
	Object* PurpleEnemyObj3 = Engine::GetSpawner().Spawn("PurpleEnemy");
	PurpleEnemyObj3->SetLocation(PurpleEnemyStart3);
	PurpleEnemyObj3->SetSize(TmComp->GetTileSize().Y * 2.5f, TmComp->GetTileSize().Y * 2.5f);
	mObjectsToAddToWorld.push_back(PurpleEnemyObj3);

	// Flying enemy 1
	Vector<float> FlyingEnemyTileStart(22, 6);
	Vector<float> FlyingEnemyStart = Vector<float>(FlyingEnemyTileStart.X * TmComp->GetTileSize().X, FlyingEnemyTileStart.Y * TmComp->GetTileSize().Y);
	Object* FlyingEnemyObj = Engine::GetSpawner().Spawn("FlyingEnemy");
	FlyingEnemyObj->SetLocation(FlyingEnemyStart);
	FlyingEnemyObj->SetSize(TmComp->GetTileSize().Y * 2, TmComp->GetTileSize().Y * 2);
	mObjectsToAddToWorld.push_back(FlyingEnemyObj);

	// Flying enemy 2
	Vector<float> FlyingEnemyTileStart2(19, 12);
	Vector<float> FlyingEnemyStart2 = Vector<float>(FlyingEnemyTileStart2.X * TmComp->GetTileSize().X, FlyingEnemyTileStart2.Y * TmComp->GetTileSize().Y);
	Object* FlyingEnemyObj2 = Engine::GetSpawner().Spawn("FlyingEnemy");
	FlyingEnemyObj2->SetLocation(FlyingEnemyStart2);
	FlyingEnemyObj2->SetSize(TmComp->GetTileSize().Y * 2, TmComp->GetTileSize().Y * 2);
	mObjectsToAddToWorld.push_back(FlyingEnemyObj2);

	// Flying enemy 3
	Vector<float> FlyingEnemyTileStart3(17, 9);
	Vector<float> FlyingEnemyStart3 = Vector<float>(FlyingEnemyTileStart3.X * TmComp->GetTileSize().X, FlyingEnemyTileStart3.Y * TmComp->GetTileSize().Y);
	Object* FlyingEnemyObj3 = Engine::GetSpawner().Spawn("FlyingEnemy");
	FlyingEnemyObj3->SetLocation(FlyingEnemyStart3);
	FlyingEnemyObj3->SetSize(TmComp->GetTileSize().Y * 2, TmComp->GetTileSize().Y * 2);
	mObjectsToAddToWorld.push_back(FlyingEnemyObj3);

	// Flying enemy 4
	Vector<float> FlyingEnemyTileStart4(13, 8);
	Vector<float> FlyingEnemyStart4 = Vector<float>(FlyingEnemyTileStart4.X * TmComp->GetTileSize().X, FlyingEnemyTileStart4.Y * TmComp->GetTileSize().Y);
	Object* FlyingEnemyObj4 = Engine::GetSpawner().Spawn("FlyingEnemy");
	FlyingEnemyObj4->SetLocation(FlyingEnemyStart4);
	FlyingEnemyObj4->SetSize(TmComp->GetTileSize().Y * 2, TmComp->GetTileSize().Y * 2);
	mObjectsToAddToWorld.push_back(FlyingEnemyObj4);

	// Gate
	Vector<float> GateTileStart(22, 6);
	Vector<float> GateStart = Vector<float>(GateTileStart.X * TmComp->GetTileSize().X, GateTileStart.Y * TmComp->GetTileSize().Y);
	Object* GateObj = Engine::GetSpawner().Spawn("Gate");
	GateObj->SetLocation(GateStart);
	GateObj->SetSize(TmComp->GetTileSize().Y * 3.f, TmComp->GetTileSize().Y * 3.5f);
	mObjectsToAddToWorld.push_back(GateObj);

	Vector<float> ChangeSceneTriggerTileStart(24, 6);
	Vector<float> ChangeSceneTriggerStart = Vector<float>(ChangeSceneTriggerTileStart.X * TmComp->GetTileSize().X, ChangeSceneTriggerTileStart.Y * TmComp->GetTileSize().Y);
	Object* ChangeSceneTriggerObj = Engine::GetSpawner().Spawn("ChangeSceneTrigger");
	ChangeSceneTriggerObj->SetLocation(ChangeSceneTriggerStart);
	ChangeSceneTriggerObj->SetSize(TmComp->GetTileSize().Y * 3, TmComp->GetTileSize().Y * 2);
	mObjectsToAddToWorld.push_back(ChangeSceneTriggerObj);


	/* Music Obj ----------------------------------------------- */

	Object* MusicObj = new Object();
	mObjectsToAddToWorld.push_back(MusicObj);
	MusicComponent* MusicComp = MusicObj->AddComponent<MusicComponent>();
	std::string MusicFile = ASSET_PATH + std::string("Music/ThirdSceneMusic.mp3");
	MusicComp->SetMusicFile(MusicFile);
	MusicComp->SetMusicFile(MusicFile);
	Engine::GetAudio().SetMusicVolume(MusicComp->GetMusicId(), 40);

	
	/* UI ------------------------------------------------------ */

	mObjectsToAddToWorld.push_back(Cursor);

	Object* UIObj = new Object();
	mObjectsToAddToWorld.push_back(UIObj);
	HealthBarComponent* HealthBarComp = UIObj->AddComponent<HealthBarComponent>();
	HealthBarComp->SetPaddingPercent(0.01f);
	HealthBarComp->SetSizeRatio(Vector<float>(0.2f, 0.05f));
	HealthBarComp->SetBorderSize(7);
	Player->GetComponent<AttributeComponent>()->mOnHealthChangedSubject.AddListener(HealthBarComp);


	/* Add obj in the world ------------------------------------ */

	for (Object* Obj : mObjectsToAddToWorld)
	{
		Engine::GetWorld().AddObject(Obj);
	}
}
