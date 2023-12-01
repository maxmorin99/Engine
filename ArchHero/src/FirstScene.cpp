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
#include "Components/ButtonComponent.h"
#include "Components/AttributeComponent.h"

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

	//std::vector<Tile> WallTiles = TmComp->GetTilesFromLayer("WallLayer");
	
	Vector<float> PlayerTileStart(1, 6);
	Vector<float> PlayerStart = Vector<float>(PlayerTileStart.X * TmComp->GetTileSize().X, PlayerTileStart.Y * TmComp->GetTileSize().Y);


	Object* AnimatedPlayer = Engine::GetSpawner().Spawn("Player");
	AnimatedPlayer->SetLocation(PlayerStart);
	AnimatedPlayer->SetSize(TmComp->GetTileSize().Y * 2.5f, TmComp->GetTileSize().Y * 2.5f);
	mObjectsToAddToWorld.push_back(AnimatedPlayer);

	Object* Cursor = Engine::GetSpawner().Spawn("Cursor");
	

	Object* Weapon = Engine::GetSpawner().Spawn("Weapon");
	Weapon->SetSize(AnimatedPlayer->GetSize().X * 0.24f, AnimatedPlayer->GetSize().Y * 0.12f);
	Weapon->GetComponent<WeaponComponent>()->SetOffset(Vector<float>(AnimatedPlayer->GetSize().X * 0.12f, AnimatedPlayer->GetSize().Y * 0.2f));
	Weapon->GetComponent<WeaponComponent>()->SetInstigator(AnimatedPlayer);
	Weapon->GetComponent<WeaponComponent>()->SetTargetCursorObject(Cursor);
	mObjectsToAddToWorld.push_back(Weapon);

	Vector<float> PurpleEnemyTileStart(9, 11);
	Vector<float> PurpleEnemyStart = Vector<float>(PurpleEnemyTileStart.X * TmComp->GetTileSize().X, PurpleEnemyTileStart.Y * TmComp->GetTileSize().Y);
	Object* PurpleEnemyObj = Engine::GetSpawner().Spawn("PurpleEnemy");
	PurpleEnemyObj->SetLocation(PurpleEnemyStart);
	PurpleEnemyObj->SetSize(TmComp->GetTileSize().Y * 2.5f, TmComp->GetTileSize().Y * 2.5f);
	mObjectsToAddToWorld.push_back(PurpleEnemyObj);

	Vector<float> FlyingEnemyTileStart(15, 6);
	Vector<float> FlyingEnemyStart = Vector<float>(FlyingEnemyTileStart.X * TmComp->GetTileSize().X, FlyingEnemyTileStart.Y * TmComp->GetTileSize().Y);
	Object* FlyingEnemyObj = Engine::GetSpawner().Spawn("FlyingEnemy");
	FlyingEnemyObj->SetLocation(FlyingEnemyStart);
	FlyingEnemyObj->SetSize(TmComp->GetTileSize().Y * 2, TmComp->GetTileSize().Y * 2);
	mObjectsToAddToWorld.push_back(FlyingEnemyObj);

	Vector<float> FlyingEnemyTileStart2(13, 11);
	Vector<float> FlyingEnemyStart2 = Vector<float>(FlyingEnemyTileStart2.X * TmComp->GetTileSize().X, FlyingEnemyTileStart2.Y * TmComp->GetTileSize().Y);
	Object* FlyingEnemyObj2 = Engine::GetSpawner().Spawn("FlyingEnemy");
	FlyingEnemyObj2->SetLocation(FlyingEnemyStart2);
	FlyingEnemyObj2->SetSize(TmComp->GetTileSize().Y * 2, TmComp->GetTileSize().Y * 2);
	mObjectsToAddToWorld.push_back(FlyingEnemyObj2);

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
	std::string MusicFile = ASSET_PATH + std::string("Music/FirstSceneMusic.mp3");
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
	AnimatedPlayer->GetComponent<AttributeComponent>()->mOnHealthChangedSubject.AddListener(HealthBarComp);


	/* Add obj in the world ------------------------------------ */

	for (Object* Obj : mObjectsToAddToWorld)
	{
		Engine::GetWorld().AddObject(Obj);
	}
}
