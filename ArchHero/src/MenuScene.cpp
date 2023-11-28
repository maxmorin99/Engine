#include "MenuScene.h"
#include "Object.h"
#include "Components/ButtonBoxComponent.h"
#include "Components/ButtonComponent.h"
#include "Components/TargetCursorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ButtonListenerComponent.h"
#include "Engine/Engine.h"

MenuScene::MenuScene(const char* name, const char* tilemapFile, int srcTileW, int srcTileH, int tileCountW, int tileCountH):
	Scene(name, tilemapFile, srcTileW, srcTileH, tileCountW, tileCountH)
{
}

void MenuScene::Load()
{
	int WinW, WinH = 0;
	Engine::GetGraphic().GetWindowSize(&WinW, &WinH);

	Object* BackgroundImg = new Object();
	BackgroundImg->SetSize(WinW, WinH);
	mObjectsToAddToWorld.push_back(BackgroundImg);
	SpriteComponent* SpriteComp = BackgroundImg->AddComponent<SpriteComponent>();
	std::string ImgFile = ASSET_PATH + std::string("MenuImage.jpg");
	SpriteComp->SetFile(ImgFile);

	// Button box
	Object* ButtonBoxObj = new Object();
	mObjectsToAddToWorld.push_back(ButtonBoxObj);
	
	ButtonBoxComponent* ButtonBoxComp = ButtonBoxObj->AddComponent<ButtonBoxComponent>();
	float BoxW = WinW * 0.4f;
	float BoxH = WinH * 0.5f;
	float BoxX = (WinW / 2) - (BoxW / 2);
	float BoxY = (WinH / 2) - (BoxH / 2);
	ButtonBoxComp->SetRect(Rect<float>(BoxX, BoxY, BoxW, BoxH));
	ButtonBoxComp->SetPadding(1);

	// Play Button
	Object* PlayBtnObj = new Object();
	mObjectsToAddToWorld.push_back(PlayBtnObj);

	ButtonComponent* PlayButtonComp = PlayBtnObj->AddComponent<ButtonComponent>();
	std::string FontFile = ASSET_PATH + std::string("Font/Asset.ttf");
	PlayButtonComp->SetFont(FontFile, 24);
	PlayButtonComp->SetText("Play");
	PlayButtonComp->SetTextColor(Color::Black);
	PlayButtonComp->SetDefaultColor(Color::White);
	PlayButtonComp->SetHoverColor(Color::Grey);
	PlayButtonComp->SetBorderSize(4);
	ButtonBoxComp->AddButton(PlayButtonComp, 0.5f);

	// Quit Button
	Object* QuitBtnObj = new Object();
	mObjectsToAddToWorld.push_back(QuitBtnObj);

	ButtonComponent* QuitButtonComp = QuitBtnObj->AddComponent<ButtonComponent>();
	QuitButtonComp->SetFont(FontFile, 24);
	QuitButtonComp->SetText("Quit");
	QuitButtonComp->SetTextColor(Color::Black);
	QuitButtonComp->SetDefaultColor(Color::White);
	QuitButtonComp->SetHoverColor(Color::Grey);
	QuitButtonComp->SetBorderSize(4);
	ButtonBoxComp->AddButton(QuitButtonComp, 0.5f);

	// Button Listener
	Object* ButtonListenerObj = new Object();
	mObjectsToAddToWorld.push_back(ButtonListenerObj);

	ButtonListenerComponent* ButtonListenerComp = ButtonListenerObj->AddComponent<ButtonListenerComponent>();
	QuitButtonComp->mOnClickSubject.AddListener(ButtonListenerComp);
	PlayButtonComp->mOnClickSubject.AddListener(ButtonListenerComp);

	// Cursor
	Object* TargetCursorObj = new Object();
	mObjectsToAddToWorld.push_back(TargetCursorObj);
	TargetCursorObj->AddTag("Cursor");
	TargetCursorObj->SetLocation(0.f, 0.f);
	TargetCursorObj->SetSize(1.f, 1.f);
	TargetCursorComponent* TargetCursorComp = TargetCursorObj->AddComponent<TargetCursorComponent>();
	std::string TargetCursorFile = ASSET_PATH + std::string("Weapons/crosshair.png");
	TargetCursorComp->SetFile(TargetCursorFile);
	TargetCursorComp->SetColor(Color::Black);
	BoxComponent* TargetBoxComp = TargetCursorObj->AddComponent<BoxComponent>();
	TargetBoxComp->SetBoxSize(1.f, 1.f);
	TargetBoxComp->SetCollisionChannel(ECollisionChannel::UI);
	TargetBoxComp->SetCollisionResponseToChannel(ECollisionChannel::UI, ECollisionResponse::Overlap);

	/* Add obj in the world ------------------------------------ */

	for (Object* Obj : mObjectsToAddToWorld)
	{
		Engine::GetWorld().AddObject(Obj);
	}
}
