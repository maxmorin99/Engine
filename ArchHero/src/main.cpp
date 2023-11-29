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

using namespace Core;

void InitGameplay()
{
	// prototypes

	// Bullets -------------------------------------------------------------------------------------------- //

	// Player bullets

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


	// Enemy bullets

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
	if (Engine::Init("TestGame", 1440, 1080))
	{
		InitGameplay();
		Engine::Start();
	}

	return 0;
}