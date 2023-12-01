#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include "Engine/Core.h"
#include "Engine/Engine.h"
#include "MenuScene.h"
#include "DefaultScene.h"
#include "FirstScene.h"
#include "SecondScene.h"
#include "ThirdScene.h"

#include "Object.h"
#include "Components/BulletComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/PathFindingComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/TargetCursorComponent.h"
#include "Components/GateComponent.h"
#include "Components/ChangeSceneTrigger.h"

#include <cstdlib>  
#include <ctime>    

using namespace Core;

void InitGameplay()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	int WinW, WinH = 0;
	Engine::GetGraphic().GetWindowSize(&WinW, &WinH);

	// prototypes

	// Player bullet -------------------------------------------------------------------------------------------------- //

	Object* PlayerBullet = new Object();
	PlayerBullet->AddTag("Bullet");
	PlayerBullet->SetSize(20.f, 20.f);
	PlayerBullet->SetRotation(0.f);

	// Bullet sounds
	std::vector<size_t> BulletImpactSoundIdList;
	std::string BulletImpactSoundFile_01 = ASSET_PATH + std::string("Sfx/BulletImpactWall_01.wav");
	std::string BulletImpactSoundFile_02 = ASSET_PATH + std::string("Sfx/BulletImpactWall_02.wav");
	BulletImpactSoundIdList.push_back(Engine::GetAudio().LoadSound(BulletImpactSoundFile_01.c_str()));
	BulletImpactSoundIdList.push_back(Engine::GetAudio().LoadSound(BulletImpactSoundFile_02.c_str()));
	for (int i = 0; i < BulletImpactSoundIdList.size(); i++)
	{
		Engine::GetAudio().SetSFXVolume(BulletImpactSoundIdList[i], 7);
	}

	std::vector<size_t> BulletImpactBodySoundIdList;
	std::string BulletImpactBodySoundFile_01 = ASSET_PATH + std::string("Sfx/BulletImpactBody_01.wav");
	std::string BulletImpactBodySoundFile_02 = ASSET_PATH + std::string("Sfx/BulletImpactBody_02.wav");
	std::string BulletImpactBodySoundFile_03 = ASSET_PATH + std::string("Sfx/BulletImpactBody_03.wav");
	std::string BulletImpactBodySoundFile_04 = ASSET_PATH + std::string("Sfx/BulletImpactBody_04.wav");
	BulletImpactBodySoundIdList.push_back(Engine::GetAudio().LoadSound(BulletImpactBodySoundFile_01.c_str()));
	BulletImpactBodySoundIdList.push_back(Engine::GetAudio().LoadSound(BulletImpactBodySoundFile_02.c_str()));
	BulletImpactBodySoundIdList.push_back(Engine::GetAudio().LoadSound(BulletImpactBodySoundFile_03.c_str()));
	BulletImpactBodySoundIdList.push_back(Engine::GetAudio().LoadSound(BulletImpactBodySoundFile_04.c_str()));
	for (int i = 0; i < BulletImpactBodySoundIdList.size(); i++)
	{
		Engine::GetAudio().SetSFXVolume(BulletImpactBodySoundIdList[i], 25);
	}

	BulletComponent* PlayerBulletComp = PlayerBullet->AddComponent<BulletComponent>();
	PlayerBulletComp->SetDamage(10.f);
	PlayerBulletComp->AddImpactWallSoundId(BulletImpactSoundIdList);
	PlayerBulletComp->AddImpactBodySoundId(BulletImpactBodySoundIdList);

	// Px
	PhysicComponent* PlayerPxComp = PlayerBullet->AddComponent<PhysicComponent>();
	PlayerPxComp->SetMass(1.f);
	PlayerPxComp->SetSlideFactor(0);
	PlayerPxComp->SetMaxMovementSpeed(750);
	PlayerPxComp->SetAccelerationSpeed(10000);
	PlayerPxComp->SetDecelerationSpeed(10000);

	// Collision
	BoxComponent* PlayerBoxComp = PlayerBullet->AddComponent<BoxComponent>();
	PlayerBoxComp->SetCollisionChannel(ECollisionChannel::Projectile);
	PlayerBoxComp->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Overlap);
	PlayerBoxComp->SetCollisionResponseToChannel(ECollisionChannel::Enemy, ECollisionResponse::Overlap);
	PlayerBoxComp->SetBoxSize(20.f, 20.f);

	// Animation
	AnimationComponent* PlayerBulletAnim = PlayerBullet->AddComponent<AnimationComponent>();
	std::string BulletFile = ASSET_PATH + std::string("Projectiles/PlayerProjectile2.png");
	PlayerBulletAnim->SetFile(BulletFile);
	std::vector<Frame> BulletIdleFrames;
	BulletIdleFrames.push_back(Frame(3, 0, 256, 256, "Idle_1"));
	BulletIdleFrames.push_back(Frame(0, 1, 256, 256, "Idle_2"));
	BulletIdleFrames.push_back(Frame(1, 1, 256, 256, "Idle_3"));
	BulletIdleFrames.push_back(Frame(2, 1, 256, 256, "Idle_4"));
	BulletIdleFrames.push_back(Frame(3, 1, 256, 256, "Idle_5"));
	Clip IdleClip("IdleClip", BulletIdleFrames, 0.1f);
	PlayerBulletAnim->AddClip("IdleClip", IdleClip);

	std::vector<Frame> BulletExplosionFrames;
	BulletExplosionFrames.push_back(Frame(4, 0, 256, 256, "Explosion_1"));
	BulletExplosionFrames.push_back(Frame(5, 0, 256, 256, "Explosion_2"));
	BulletExplosionFrames.push_back(Frame(6, 0, 256, 256, "Explosion_3"));
	BulletExplosionFrames.push_back(Frame(7, 0, 256, 256, "Explosion_4"));
	Clip ExplosionClip("ExplosionClip", BulletExplosionFrames, 0.005f);
	PlayerBulletAnim->AddClip("ExplosionClip", ExplosionClip);

	std::vector<Frame> BulletBloodFrames;
	BulletBloodFrames.push_back(Frame(8, 0, 256, 256, "Blood_1"));
	BulletBloodFrames.push_back(Frame(9, 0, 256, 256, "Blood_2"));
	BulletBloodFrames.push_back(Frame(10, 0, 256, 256, "Blood_3"));
	BulletBloodFrames.push_back(Frame(11, 0, 256, 256, "Blood_4"));
	BulletBloodFrames.push_back(Frame(12, 0, 256, 256, "Blood_5"));
	BulletBloodFrames.push_back(Frame(13, 0, 256, 256, "Blood_6"));
	Clip BloodClip("BloodClip", BulletBloodFrames, 0.1f);
	PlayerBulletAnim->AddClip("BloodClip", BloodClip);

	PlayerBulletAnim->SetDefaultClip(IdleClip);
	PlayerBulletAnim->SetClip("IdleClip", true);

	Engine::GetSpawner().AddPrototype("PlayerBullet", PlayerBullet);

	// ---------------------------------------------------------------------------------------------------------------- //

	// Enemy bullets -------------------------------------------------------------------------------------------------- //

	Object* EnemyBullet = new Object();
	EnemyBullet->AddTag("Bullet");
	EnemyBullet->SetSize(20.f, 20.f);
	EnemyBullet->SetRotation(0.f);
	BulletComponent* EnemyBulletComp = EnemyBullet->AddComponent<BulletComponent>();
	EnemyBulletComp->SetDamage(10.f);
	EnemyBulletComp->AddImpactWallSoundId(BulletImpactSoundIdList);
	EnemyBulletComp->AddImpactBodySoundId(BulletImpactBodySoundIdList);

	// Px
	PhysicComponent* EnemyPxComp = EnemyBullet->AddComponent<PhysicComponent>();
	EnemyPxComp->SetMass(1.f);
	EnemyPxComp->SetSlideFactor(0);
	EnemyPxComp->SetMaxMovementSpeed(750);
	EnemyPxComp->SetAccelerationSpeed(10000);
	EnemyPxComp->SetDecelerationSpeed(10000);

	// Collision
	BoxComponent* EnemyBoxComp = EnemyBullet->AddComponent<BoxComponent>();
	EnemyBoxComp->SetCollisionChannel(ECollisionChannel::Projectile);
	EnemyBoxComp->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Overlap);
	EnemyBoxComp->SetCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Overlap);
	EnemyBoxComp->SetBoxSize(20.f, 20.f);

	// Animation
	AnimationComponent* EnemyBulletAnim = EnemyBullet->AddComponent<AnimationComponent>();
	
	EnemyBulletAnim->SetFile(BulletFile);
	EnemyBulletAnim->AddClip("IdleClip", IdleClip);
	EnemyBulletAnim->AddClip("ExplosionClip", ExplosionClip);
	EnemyBulletAnim->AddClip("BloodClip", BloodClip);

	EnemyBulletAnim->SetDefaultClip(IdleClip);
	EnemyBulletAnim->SetClip("IdleClip", true);

	Engine::GetSpawner().AddPrototype("EnemyBullet", EnemyBullet);

	// ---------------------------------------------------------------------------------------------------------------- //

	// Purple Enemy --------------------------------------------------------------------------------------------------- //

	Object* PurpleEnemy = new Object();
	PurpleEnemy->AddTag("Enemy");
	PurpleEnemy->AddTag("Character");
	PurpleEnemy->SetLocation(900, 450);
	PurpleEnemy->SetSize(150, 150);

	// Sounds
	std::vector<size_t> PurpleEnemyDeathSoundIdList;
	std::string PurpleEnemyDeathFile_01 = ASSET_PATH + std::string("Sfx/PurpleCreatureDeath_01.wav");
	std::string PurpleEnemyDeathFile_02 = ASSET_PATH + std::string("Sfx/PurpleCreatureDeath_02.wav");
	std::string PurpleEnemyDeathFile_03 = ASSET_PATH + std::string("Sfx/PurpleCreatureDeath_03.wav");
	PurpleEnemyDeathSoundIdList.push_back(Engine::GetAudio().LoadSound(PurpleEnemyDeathFile_01.c_str()));
	PurpleEnemyDeathSoundIdList.push_back(Engine::GetAudio().LoadSound(PurpleEnemyDeathFile_02.c_str()));
	PurpleEnemyDeathSoundIdList.push_back(Engine::GetAudio().LoadSound(PurpleEnemyDeathFile_03.c_str()));
	for (int i = 0; i < PurpleEnemyDeathSoundIdList.size(); i++)
	{
		Engine::GetAudio().SetSFXVolume(PurpleEnemyDeathSoundIdList[i], 35);
	}

	EnemyComponent* PurpleEnemyComp = PurpleEnemy->AddComponent<EnemyComponent>();
	PurpleEnemyComp->AddDeathAudioIdList(PurpleEnemyDeathSoundIdList);
	PurpleEnemyComp->SetToleranceDistance(WinW * 0.09f);
	PurpleEnemyComp->SetAttackDelay(2.f);
	PurpleEnemyComp->SetHitFlashTime(0.15f);
	PathFindingComponent* PurplePathComp = PurpleEnemy->AddComponent<PathFindingComponent>();

	// Attribute
	AttributeComponent* PurpleEnemyAttribute = PurpleEnemy->AddComponent<AttributeComponent>();
	PurpleEnemyAttribute->SetMaxHealth(50.f);
	PurpleEnemyAttribute->mOnHealthChangedSubject.AddListener(PurpleEnemyComp);

	// Px
	PhysicComponent* PurpleEnemyPxComp = PurpleEnemy->AddComponent<PhysicComponent>();
	PurpleEnemyPxComp->SetMass(1.f);
	PurpleEnemyPxComp->SetSlideFactor(0);
	PurpleEnemyPxComp->SetMaxMovementSpeed(100.f);
	PurpleEnemyPxComp->SetAccelerationSpeed(10000);
	PurpleEnemyPxComp->SetDecelerationSpeed(10000);

	// Enemy1 collision
	BoxComponent* PurpleEnemyBox = PurpleEnemy->AddComponent<BoxComponent>();
	PurpleEnemyBox->SetCollisionChannel(ECollisionChannel::Enemy);
	PurpleEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Ignore);
	PurpleEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Ignore);
	PurpleEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Enemy, ECollisionResponse::Ignore);
	PurpleEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Projectile, ECollisionResponse::Overlap);
	PurpleEnemyBox->SetBoxSize(75, 100);
	PurpleEnemyBox->SetOffset(85, 120);

	// Animation
	AnimationComponent* PurpleEnemyAnimationComp = PurpleEnemy->AddComponent<AnimationComponent>();
	std::string PurpleEnemySpriteSheet = ASSET_PATH + std::string("Enemies/Enemy_1_Sprite_Sheet.png");
	PurpleEnemyAnimationComp->SetFile(PurpleEnemySpriteSheet);
	PurpleEnemyAnimationComp->SetCenterOffset(Vector<float>(0, 50));

	// Enemy1 Idle
	std::vector<Frame> PurpleEnemyIdleFrames;
	PurpleEnemyIdleFrames.push_back(Frame(0, 2, 2048, 2048, "Idle_1"));
	PurpleEnemyIdleFrames.push_back(Frame(1, 2, 2048, 2048, "Idle_2"));
	PurpleEnemyIdleFrames.push_back(Frame(2, 2, 2048, 2048, "Idle_3"));
	PurpleEnemyIdleFrames.push_back(Frame(3, 2, 2048, 2048, "Idle_4"));
	PurpleEnemyIdleFrames.push_back(Frame(4, 2, 2048, 2048, "Idle_5"));
	PurpleEnemyIdleFrames.push_back(Frame(0, 3, 2048, 2048, "Idle_6"));
	Clip PurpleEnemyIdleClip("Idle", PurpleEnemyIdleFrames, 0.1f);
	PurpleEnemyAnimationComp->AddClip("Idle", PurpleEnemyIdleClip);

	// Enemy1 Walk
	std::vector<Frame> PurpleEnemyWalkFrames;
	PurpleEnemyWalkFrames.push_back(Frame(1, 3, 2048, 2048, "Walk_1"));
	PurpleEnemyWalkFrames.push_back(Frame(2, 3, 2048, 2048, "Walk_2"));
	PurpleEnemyWalkFrames.push_back(Frame(3, 3, 2048, 2048, "Walk_3"));
	PurpleEnemyWalkFrames.push_back(Frame(4, 3, 2048, 2048, "Walk_4"));
	PurpleEnemyWalkFrames.push_back(Frame(0, 4, 2048, 2048, "Walk_5"));
	PurpleEnemyWalkFrames.push_back(Frame(1, 4, 2048, 2048, "Walk_6"));
	PurpleEnemyWalkFrames.push_back(Frame(2, 4, 2048, 2048, "Walk_7"));
	PurpleEnemyWalkFrames.push_back(Frame(3, 4, 2048, 2048, "Walk_8"));
	Clip PurpleEnemyWalkClip("Walk", PurpleEnemyWalkFrames, 0.1f);
	PurpleEnemyAnimationComp->AddClip("Walk", PurpleEnemyWalkClip);

	// Enemy1 Death
	std::vector<Frame> PurpleEnemyDeathFrames;
	PurpleEnemyDeathFrames.push_back(Frame(0, 0, 2048, 2048, "Death_1"));
	PurpleEnemyDeathFrames.push_back(Frame(1, 0, 2048, 2048, "Death_2"));
	PurpleEnemyDeathFrames.push_back(Frame(2, 0, 2048, 2048, "Death_3"));
	PurpleEnemyDeathFrames.push_back(Frame(3, 0, 2048, 2048, "Death_4"));
	PurpleEnemyDeathFrames.push_back(Frame(4, 0, 2048, 2048, "Death_5"));
	PurpleEnemyDeathFrames.push_back(Frame(0, 1, 2048, 2048, "Death_6"));
	PurpleEnemyDeathFrames.push_back(Frame(1, 1, 2048, 2048, "Death_7"));
	PurpleEnemyDeathFrames.push_back(Frame(2, 1, 2048, 2048, "Death_8"));
	PurpleEnemyDeathFrames.push_back(Frame(3, 1, 2048, 2048, "Death_9"));
	PurpleEnemyDeathFrames.push_back(Frame(4, 1, 2048, 2048, "Death_10"));
	Clip Enemy1DeathClip("Death", PurpleEnemyDeathFrames, 0.05f);
	PurpleEnemyAnimationComp->AddClip("Death", Enemy1DeathClip);

	PurpleEnemyAnimationComp->SetDefaultClip(PurpleEnemyIdleClip);
	PurpleEnemyAnimationComp->SetClip("Idle", true);

	Engine::GetSpawner().AddPrototype("PurpleEnemy", PurpleEnemy);

	// ---------------------------------------------------------------------------------------------------------------- //

	// Flying Enemy --------------------------------------------------------------------------------------------------- //

	Object* FlyingEnemy = new Object();
	FlyingEnemy->AddTag("Enemy");
	FlyingEnemy->AddTag("Character");
	FlyingEnemy->SetLocation(900, 700);
	FlyingEnemy->SetSize(150, 150); //250, 250

	// Sounds
	std::vector<size_t> FlyingEnemyDeathIdList;
	std::string FlyingEnemyDeathSound_01 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_01.wav");
	std::string FlyingEnemyDeathSound_02 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_02.wav");
	std::string FlyingEnemyDeathSound_03 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_03.wav");
	std::string FlyingEnemyDeathSound_04 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_04.wav");
	std::string FlyingEnemyDeathSound_05 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_05.wav");
	std::string FlyingEnemyDeathSound_06 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_06.wav");
	std::string FlyingEnemyDeathSound_07 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_07.wav");
	std::string FlyingEnemyDeathSound_08 = ASSET_PATH + std::string("Sfx/FlyingCreatureDeath_08.wav");
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_01.c_str()));
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_02.c_str()));
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_03.c_str()));
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_04.c_str()));
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_05.c_str()));
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_06.c_str()));
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_07.c_str()));
	FlyingEnemyDeathIdList.push_back(Engine::GetAudio().LoadSound(FlyingEnemyDeathSound_08.c_str()));
	for (int i = 0; i < FlyingEnemyDeathIdList.size(); i++)
	{
		Engine::GetAudio().SetSFXVolume(FlyingEnemyDeathIdList[i], 35);
	}

	EnemyComponent* FlyingEnemyComp = FlyingEnemy->AddComponent<EnemyComponent>();
	FlyingEnemyComp->AddDeathAudioIdList(FlyingEnemyDeathIdList);
	FlyingEnemyComp->SetHitFlashTime(0.15f);
	FlyingEnemyComp->SetToleranceDistance(WinW * 0.3f);
	FlyingEnemyComp->SetAttackDelay(2.f);
	PathFindingComponent* FlyingPathComp = FlyingEnemy->AddComponent<PathFindingComponent>();

	// Attribute
	AttributeComponent* FlyingEnemyAttribute = FlyingEnemy->AddComponent<AttributeComponent>();
	FlyingEnemyAttribute->SetMaxHealth(50.f);
	FlyingEnemyAttribute->mOnHealthChangedSubject.AddListener(FlyingEnemyComp);

	// Px
	PhysicComponent* FlyingEnemyPxComp = FlyingEnemy->AddComponent<PhysicComponent>();
	FlyingEnemyPxComp->SetMass(1.f);
	FlyingEnemyPxComp->SetSlideFactor(0);
	FlyingEnemyPxComp->SetMaxMovementSpeed(100.f);
	FlyingEnemyPxComp->SetAccelerationSpeed(10000);
	FlyingEnemyPxComp->SetDecelerationSpeed(10000);

	// Flying Enemy collision
	BoxComponent* FlyingEnemyBox = FlyingEnemy->AddComponent<BoxComponent>();
	FlyingEnemyBox->SetCollisionChannel(ECollisionChannel::Enemy);
	FlyingEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Ignore);
	FlyingEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Ignore);
	FlyingEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Enemy, ECollisionResponse::Ignore);
	FlyingEnemyBox->SetCollisionResponseToChannel(ECollisionChannel::Projectile, ECollisionResponse::Overlap);
	FlyingEnemyBox->SetBoxSize(75, 100);
	FlyingEnemyBox->SetOffset(85, 120);

	// Animation
	AnimationComponent* FlyingEnemyAnimationComp = FlyingEnemy->AddComponent<AnimationComponent>();
	std::string FlyingEnemySpriteSheet = ASSET_PATH + std::string("Enemies/FlyingEnemy_Sprite_Sheet.png");
	FlyingEnemyAnimationComp->SetFile(FlyingEnemySpriteSheet);
	FlyingEnemyAnimationComp->SetCenterOffset(Vector<float>(0, 50));
	FlyingEnemyAnimationComp->SetDeathAnimationColor(Color::Yellow);

	// Flying enemy fly
	std::vector<Frame> FlyingEnemyFlyFrames;
	FlyingEnemyFlyFrames.push_back(Frame(0, 0, 256, 256, "Fly_1"));
	FlyingEnemyFlyFrames.push_back(Frame(1, 0, 256, 256, "Fly_2"));
	FlyingEnemyFlyFrames.push_back(Frame(2, 0, 256, 256, "Fly_3"));
	FlyingEnemyFlyFrames.push_back(Frame(3, 0, 256, 256, "Fly_4"));
	FlyingEnemyFlyFrames.push_back(Frame(4, 0, 256, 256, "Fly_5"));
	FlyingEnemyFlyFrames.push_back(Frame(5, 0, 256, 256, "Fly_6"));

	Clip FlyingEnemyFlyClip("Fly", FlyingEnemyFlyFrames, 0.1f);
	FlyingEnemyAnimationComp->AddClip("Fly", FlyingEnemyFlyClip);

	std::vector<Frame> FlyingEnemyDeathFrames;
	FlyingEnemyDeathFrames.push_back(Frame(1, 1, 256, 256, "Fly_1"));
	FlyingEnemyDeathFrames.push_back(Frame(2, 1, 256, 256, "Fly_2"));
	FlyingEnemyDeathFrames.push_back(Frame(3, 1, 256, 256, "Fly_3"));
	FlyingEnemyDeathFrames.push_back(Frame(0, 2, 256, 256, "Fly_4"));
	FlyingEnemyDeathFrames.push_back(Frame(1, 2, 256, 256, "Fly_5"));
	FlyingEnemyDeathFrames.push_back(Frame(2, 2, 256, 256, "Fly_6"));
	FlyingEnemyDeathFrames.push_back(Frame(3, 2, 256, 256, "Fly_7"));
	FlyingEnemyDeathFrames.push_back(Frame(0, 3, 256, 256, "Fly_8"));
	FlyingEnemyDeathFrames.push_back(Frame(1, 3, 256, 256, "Fly_9"));
	FlyingEnemyDeathFrames.push_back(Frame(2, 3, 256, 256, "Fly_10"));
	FlyingEnemyDeathFrames.push_back(Frame(3, 3, 256, 256, "Fly_11"));
	FlyingEnemyDeathFrames.push_back(Frame(0, 4, 256, 256, "Fly_12"));
	FlyingEnemyDeathFrames.push_back(Frame(1, 4, 256, 256, "Fly_13"));

	Clip FlyingEnemyDeathClip("Death", FlyingEnemyDeathFrames, 0.05f);
	FlyingEnemyAnimationComp->AddClip("Death", FlyingEnemyDeathClip);

	FlyingEnemyAnimationComp->SetDefaultClip(FlyingEnemyFlyClip);
	FlyingEnemyAnimationComp->SetClip("Fly", true);

	Engine::GetSpawner().AddPrototype("FlyingEnemy", FlyingEnemy);
	
	// ---------------------------------------------------------------------------------------------------------------- //

	// Player --------------------------------------------------------------------------------------------------------- //

	std::vector<size_t> PlayerHitIdList;
	std::string PlayerHitSoundFile_01 = ASSET_PATH + std::string("Sfx/PlayerHit_01.wav");
	std::string PlayerHitSoundFile_02 = ASSET_PATH + std::string("Sfx/PlayerHit_02.wav");
	std::string PlayerHitSoundFile_03 = ASSET_PATH + std::string("Sfx/PlayerHit_03.wav");

	// Sounds
	PlayerHitIdList.push_back(Engine::GetAudio().LoadSound(PlayerHitSoundFile_01.c_str()));
	PlayerHitIdList.push_back(Engine::GetAudio().LoadSound(PlayerHitSoundFile_02.c_str()));
	PlayerHitIdList.push_back(Engine::GetAudio().LoadSound(PlayerHitSoundFile_03.c_str()));
	for (int i = 0; i < PlayerHitIdList.size(); i++)
	{
		Engine::GetAudio().SetSFXVolume(PlayerHitIdList[i], 20);
	}
	std::string PlayerDeathSoundFile = ASSET_PATH + std::string("Sfx/PlayerDeath.wav");

	Object* Player = new Object();
	Player->AddTag("Player");
	Player->AddTag("Character");
	PlayerComponent* PlayerComp = Player->AddComponent<PlayerComponent>();
	PlayerComp->AddHitSoundId(PlayerHitIdList);
	PlayerComp->SetDeathSoundId(Engine::GetAudio().LoadSound(PlayerDeathSoundFile.c_str()));
	Engine::GetAudio().SetSFXVolume(PlayerComp->GetDeathSondId(), 20);
	Player->SetLocation(250.f, 250.f);
	Player->SetSize(250, 250);
	AttributeComponent* PlayerAttribute = Player->AddComponent<AttributeComponent>();
	PlayerAttribute->SetMaxHealth(100.f);

	// Physic
	PhysicComponent* PlayerPx = Player->AddComponent<PhysicComponent>();
	PlayerPx->SetMass(1.f);
	PlayerPx->SetSlideFactor(0);
	PlayerPx->SetMaxMovementSpeed(250.f);
	PlayerPx->SetAccelerationSpeed(10000);
	PlayerPx->SetDecelerationSpeed(10000);

	BoxComponent* PlayerBox = Player->AddComponent<BoxComponent>();
	PlayerBox->SetCollisionChannel(ECollisionChannel::Player);
	PlayerBox->SetCollisionResponseToChannel(ECollisionChannel::World, ECollisionResponse::Block);
	PlayerBox->SetCollisionResponseToChannel(ECollisionChannel::Enemy, ECollisionResponse::Ignore);
	PlayerBox->SetCollisionResponseToChannel(ECollisionChannel::Projectile, ECollisionResponse::Overlap);

	// Animation
	AnimationComponent* PlayerAnim = Player->AddComponent<AnimationComponent>();
	std::string FilePath = ASSET_PATH + std::string("Character_Sprite_Sheet.png");
	PlayerAnim->SetFile(FilePath);
	PlayerAnim->SetCenterOffset(Vector<float>(0, 50));

	// Idle animation
	std::vector<Frame> PlayerIdleFrames;
	PlayerIdleFrames.push_back(Frame(4, 1, 2048, 2048, "Idle_1"));
	PlayerIdleFrames.push_back(Frame(5, 1, 2048, 2048, "Idle_2"));
	PlayerIdleFrames.push_back(Frame(0, 2, 2048, 2048, "Idle_3"));
	PlayerIdleFrames.push_back(Frame(1, 2, 2048, 2048, "Idle_4"));
	PlayerIdleFrames.push_back(Frame(2, 2, 2048, 2048, "Idle_5"));
	PlayerIdleFrames.push_back(Frame(3, 2, 2048, 2048, "Idle_6"));
	Clip PlayerIdleClip = Clip("Idle", PlayerIdleFrames, 0.1f);
	PlayerAnim->AddClip("Idle", PlayerIdleClip);

	// walk animation
	std::vector<Frame> PlayerWalkFrames;
	PlayerWalkFrames.push_back(Frame(3, 3, 2048, 2048, "Walk_1"));
	PlayerWalkFrames.push_back(Frame(4, 3, 2048, 2048, "Walk_2"));
	PlayerWalkFrames.push_back(Frame(5, 3, 2048, 2048, "Walk_3"));
	PlayerWalkFrames.push_back(Frame(0, 4, 2048, 2048, "Walk_4"));
	PlayerWalkFrames.push_back(Frame(1, 4, 2048, 2048, "Walk_5"));
	PlayerWalkFrames.push_back(Frame(2, 4, 2048, 2048, "Walk_6"));
	PlayerWalkFrames.push_back(Frame(3, 4, 2048, 2048, "Walk_7"));
	PlayerWalkFrames.push_back(Frame(4, 4, 2048, 2048, "Walk_8"));
	Clip PlayerWalkClip = Clip("Walk", PlayerWalkFrames, 0.05f);
	PlayerAnim->AddClip("Walk", PlayerWalkClip);

	// Death animation
	std::vector<Frame> PlayerDeathFrames;
	PlayerDeathFrames.push_back(Frame(0, 0, 2048, 2048, "Death_1"));
	PlayerDeathFrames.push_back(Frame(1, 0, 2048, 2048, "Death_2"));
	PlayerDeathFrames.push_back(Frame(2, 0, 2048, 2048, "Death_3"));
	PlayerDeathFrames.push_back(Frame(3, 0, 2048, 2048, "Death_4"));
	PlayerDeathFrames.push_back(Frame(4, 0, 2048, 2048, "Death_5"));
	PlayerDeathFrames.push_back(Frame(5, 0, 2048, 2048, "Death_6"));
	PlayerDeathFrames.push_back(Frame(0, 1, 2048, 2048, "Death_7"));
	PlayerDeathFrames.push_back(Frame(1, 1, 2048, 2048, "Death_8"));
	PlayerDeathFrames.push_back(Frame(2, 1, 2048, 2048, "Death_9"));
	PlayerDeathFrames.push_back(Frame(3, 1, 2048, 2048, "Death_10"));
	Clip PlayerDeathClip = Clip("Death", PlayerDeathFrames, 0.065f);
	PlayerAnim->AddClip("Death", PlayerDeathClip);

	// Roll animation
	std::vector<Frame> PlayerRollFrames;
	PlayerRollFrames.push_back(Frame(4, 2, 2048, 2048, "Roll_1"));
	PlayerRollFrames.push_back(Frame(5, 2, 2048, 2048, "Roll_2"));
	PlayerRollFrames.push_back(Frame(0, 3, 2048, 2048, "Roll_3"));
	PlayerRollFrames.push_back(Frame(1, 3, 2048, 2048, "Roll_4"));
	PlayerRollFrames.push_back(Frame(2, 3, 2048, 2048, "Roll_5"));
	Clip PlayerRollClip = Clip("Roll", PlayerRollFrames, 0.065f);
	PlayerAnim->AddClip("Roll", PlayerRollClip);

	PlayerAnim->SetDefaultClip(PlayerIdleClip);
	PlayerAnim->SetClip("Idle", true);

	Engine::GetSpawner().AddPrototype("Player", Player);

	// ---------------------------------------------------------------------------------------------------------------- //

	// Weapon --------------------------------------------------------------------------------------------------------- //

	Object* WeaponObj = new Object();
	WeaponObj->SetLocation(300, 300);
	WeaponObj->SetSize(60, 30);

	WeaponComponent* WeaponComp = WeaponObj->AddComponent<WeaponComponent>();
	std::string WeaponFile = ASSET_PATH + std::string("Weapons/testweapon.png");
	WeaponComp->SetFile(WeaponFile);
	//WeaponComp->SetInstigator(AnimatedPlayer);
	WeaponComp->SetColor(Color::Black);
	WeaponComp->SetOffset(Vector<float>(30.f, 50.f));

	Engine::GetSpawner().AddPrototype("Weapon", WeaponObj);

	// ---------------------------------------------------------------------------------------------------------------- //

	// Cursor --------------------------------------------------------------------------------------------------------- //

	Object* TargetCursorObj = new Object();
	TargetCursorObj->AddTag("Cursor");
	TargetCursorObj->SetLocation(0.f, 0.f);
	TargetCursorObj->SetSize(25.f, 25.f);
	TargetCursorComponent* TargetCursorComp = TargetCursorObj->AddComponent<TargetCursorComponent>();
	std::string TargetCursorFile = ASSET_PATH + std::string("Weapons/crosshair.png");
	TargetCursorComp->SetFile(TargetCursorFile);
	TargetCursorComp->SetColor(Color::Black);
	BoxComponent* TargetBoxComp = TargetCursorObj->AddComponent<BoxComponent>();
	TargetBoxComp->SetBoxSize(25.f, 25.f);
	TargetBoxComp->SetCollisionChannel(ECollisionChannel::UI);
	TargetBoxComp->SetCollisionResponseToChannel(ECollisionChannel::UI, ECollisionResponse::Overlap);
	Engine::GetSpawner().AddPrototype("Cursor", TargetCursorObj);

	// ---------------------------------------------------------------------------------------------------------------- //

	// Gate ----------------------------------------------------------------------------------------------------------- //

	Object* GateObj = new Object();
	GateComponent* GateComp = GateObj->AddComponent<GateComponent>();
	const std::string GateSoundFile = ASSET_PATH + std::string("Sfx/Door.wav");
	GateComp->SetAudioFile(GateSoundFile);
	GateObj->SetSize(150, 150);
	GateObj->SetLocation(0.f, 0.f);
	BoxComponent* GateBox = GateObj->AddComponent<BoxComponent>();
	GateBox->SetCollisionChannel(ECollisionChannel::World);
	GateBox->SetCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Block);
	AnimationComponent* GateAnimComp = GateObj->AddComponent<AnimationComponent>();
	const std::string GateFile = ASSET_PATH + std::string("Gate/Gate.png");
	GateAnimComp->SetFile(GateFile);

	std::vector<Frame> GateIdleFrames;
	GateIdleFrames.push_back(Frame(0, 0, 128, 160, "Idle_1"));

	Clip GateIdleClip("IdleClip", GateIdleFrames, 0);
	GateAnimComp->AddClip("Idle", GateIdleClip);

	std::vector<Frame> GateOpenedFrames;
	GateOpenedFrames.push_back(Frame(22, 0, 128, 160, "Opened_1"));

	Clip GateOpenedClip("OpenedClip", GateOpenedFrames, 0);
	GateAnimComp->AddClip("Opened", GateOpenedClip);

	std::vector<Frame> GateOpenFrames;
	GateOpenFrames.push_back(Frame(0, 0, 128, 160, "Open_1"));
	GateOpenFrames.push_back(Frame(1, 0, 128, 160, "Open_2"));
	GateOpenFrames.push_back(Frame(2, 0, 128, 160, "Open_3"));
	GateOpenFrames.push_back(Frame(3, 0, 128, 160, "Open_4"));
	GateOpenFrames.push_back(Frame(4, 0, 128, 160, "Open_5"));
	GateOpenFrames.push_back(Frame(5, 0, 128, 160, "Open_6"));
	GateOpenFrames.push_back(Frame(6, 0, 128, 160, "Open_7"));
	GateOpenFrames.push_back(Frame(7, 0, 128, 160, "Open_8"));
	GateOpenFrames.push_back(Frame(8, 0, 128, 160, "Open_9"));
	GateOpenFrames.push_back(Frame(9, 0, 128, 160, "Open_10"));
	GateOpenFrames.push_back(Frame(10, 0, 128, 160, "Open_11"));
	GateOpenFrames.push_back(Frame(11, 0, 128, 160, "Open_12"));
	GateOpenFrames.push_back(Frame(12, 0, 128, 160, "Open_13"));
	GateOpenFrames.push_back(Frame(13, 0, 128, 160, "Open_14"));
	GateOpenFrames.push_back(Frame(14, 0, 128, 160, "Open_15"));
	GateOpenFrames.push_back(Frame(15, 0, 128, 160, "Open_16"));
	GateOpenFrames.push_back(Frame(16, 0, 128, 160, "Open_17"));
	GateOpenFrames.push_back(Frame(17, 0, 128, 160, "Open_18"));
	GateOpenFrames.push_back(Frame(18, 0, 128, 160, "Open_19"));
	GateOpenFrames.push_back(Frame(19, 0, 128, 160, "Open_20"));
	GateOpenFrames.push_back(Frame(20, 0, 128, 160, "Open_21"));
	GateOpenFrames.push_back(Frame(21, 0, 128, 160, "Open_22"));
	GateOpenFrames.push_back(Frame(22, 0, 128, 160, "Open_23"));

	Clip GateOpenClip("OpenClip", GateOpenFrames, 0.08f);
	GateAnimComp->AddClip("Open", GateOpenClip);

	GateAnimComp->SetDefaultClip(GateOpenedClip);
	GateAnimComp->SetClip("Idle", true);
	Engine::GetSpawner().AddPrototype("Gate", GateObj);

	// ---------------------------------------------------------------------------------------------------------------- //

	// ChangeSceneTrigger --------------------------------------------------------------------------------------------- //

	Object* ChangeSceneTriggerObj = new Object();
	ChangeSceneTriggerObj->AddComponent<ChangeSceneTrigger>();
	BoxComponent* ChangeSceneTriggerBoxComp = ChangeSceneTriggerObj->AddComponent<BoxComponent>();
	ChangeSceneTriggerBoxComp->SetCollisionChannel(ECollisionChannel::World);
	ChangeSceneTriggerBoxComp->SetCollisionResponseToChannel(ECollisionChannel::Player, ECollisionResponse::Overlap);
	Engine::GetSpawner().AddPrototype("ChangeSceneTrigger", ChangeSceneTriggerObj);

	// ---------------------------------------------------------------------------------------------------------------- //


	// scenes
	std::string FirstSceneTiledFile = ASSET_PATH + std::string("PrisonTileset/Map_1.tmx");
	std::string SecondSceneTiledFile = ASSET_PATH + std::string("PrisonTileset/Map_2.tmx");
	std::string ThirdSceneTiledFile = ASSET_PATH + std::string("PrisonTileset/Map_3.tmx");
	IScene* Menu = new MenuScene("MenuScene", nullptr, 0, 0, 0, 0);
	IScene* Default = new DefaultScene("DefaultScene", FirstSceneTiledFile.c_str(), 32, 32, 25, 19);
	IScene* First = new FirstScene("FirstScene", FirstSceneTiledFile.c_str(), 32, 32, 25, 19);
	IScene* Second = new SecondScene("SecondScene", SecondSceneTiledFile.c_str(), 32, 32, 25, 19);
	IScene* Third = new ThirdScene("ThirdScene", ThirdSceneTiledFile.c_str(), 32, 32, 25, 19);
	Engine::GetWorld().Register("MenuScene", Menu);
	Engine::GetWorld().Register("DefaultScene", Default);
	Engine::GetWorld().Register("FirstScene", First);
	Engine::GetWorld().Register("SecondScene", Second);
	Engine::GetWorld().Register("ThirdScene", Third);
	Engine::GetWorld().Load("MenuScene");
}

INT WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	if (Engine::Init("TestGame", 3000, 3000)) //1440 1080
	{
		InitGameplay();
		Engine::Start();
	}

	return 0;
}