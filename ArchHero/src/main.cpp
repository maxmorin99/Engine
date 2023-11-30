#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>
#include "Engine/Core.h"
#include "Engine/Engine.h"
#include "MenuScene.h"
#include "DefaultScene.h"
#include "FirstScene.h"

#include "Object.h"
#include "Components/BulletComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/PathFindingComponent.h"

using namespace Core;

void InitGameplay()
{
	// prototypes

	// Player bullet -------------------------------------------------------------------------------------------------- //

	Object* PlayerBullet = new Object();
	PlayerBullet->AddTag("Bullet");
	PlayerBullet->SetSize(20.f, 20.f);
	PlayerBullet->SetRotation(0.f);
	BulletComponent* PlayerBulletComp = PlayerBullet->AddComponent<BulletComponent>();
	PlayerBulletComp->SetDamage(10.f);

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



	// Ennemies

	// Purple Enemy --------------------------------------------------------------------------------------------------- //

	Object* PurpleEnemy = new Object();
	PurpleEnemy->AddTag("Enemy");
	PurpleEnemy->AddTag("Character");
	PurpleEnemy->SetLocation(900, 450);
	PurpleEnemy->SetSize(150, 150);

	EnemyComponent* PurpleEnemyComp = PurpleEnemy->AddComponent<EnemyComponent>();
	PurpleEnemyComp->SetToleranceDistance(275.f);
	PurpleEnemyComp->SetAttackDelay(2.f);
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

	//// ---------------------------------------------------------------------------------------------------------------- //

	//// Flying Enemy --------------------------------------------------------------------------------------------------- //

	Object* FlyingEnemy = new Object();
	FlyingEnemy->AddTag("Enemy");
	FlyingEnemy->AddTag("Character");
	FlyingEnemy->SetLocation(900, 700);
	FlyingEnemy->SetSize(150, 150); //250, 250
	EnemyComponent* FlyingEnemyComp = FlyingEnemy->AddComponent<EnemyComponent>();
	FlyingEnemyComp->SetToleranceDistance(275.f);
	FlyingEnemyComp->SetAttackDelay(2.f);
	FlyingEnemyComp->SetToleranceDistance(500);
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


	// scenes
	std::string TiledFile = ASSET_PATH + std::string("PrisonTileset/test.tmx");
	IScene* Menu = new MenuScene("MenuScene", nullptr, 0, 0, 0, 0);
	IScene* Default = new DefaultScene("DefaultScene", TiledFile.c_str(), 32, 32, 25, 19);
	IScene* First = new FirstScene("FirstScene", TiledFile.c_str(), 32, 32, 25, 19);
	Engine::GetWorld().Register("MenuScene", Menu);
	Engine::GetWorld().Register("DefaultScene", Default);
	Engine::GetWorld().Register("FirstScene", First);
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