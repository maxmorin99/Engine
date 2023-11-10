#include "Components/Functionality.h"

Core::FunctionalityComponent::FunctionalityComponent(Object* Owner) : Component(Owner)
{
}

void Core::FunctionalityComponent::Start()
{
	Component::Start();

	PlayMusic();
}

void Core::FunctionalityComponent::Draw()
{
	DrawString();
}

void Core::FunctionalityComponent::Update(float DeltaTime)
{
}

void Core::FunctionalityComponent::SetMusicFile(const char* File, int Volume)
{
	mMusicId = Audio().LoadMusic(File);
	Audio().SetMusicVolume(mMusicId, Volume);
}

void Core::FunctionalityComponent::SetFontFile(const char* File, int Size)
{
	mFontId = Graphic().LoadFont(File, Size);
}

void Core::FunctionalityComponent::SetSoundFile(const char* File, int Volume)
{
	mSoundId = Audio().LoadSound(File);
	Audio().SetSFXVolume(mSoundId, Volume);
}

void Core::FunctionalityComponent::PlayMusic() const
{
	Audio().PlayMusic(mMusicId, true);
	Audio().SetMusicVolume(mMusicId, 10);
}

void Core::FunctionalityComponent::PlaySfx() const
{
	Audio().PlaySFX(mSoundId);
}

void Core::FunctionalityComponent::DrawRect() const
{
	Rect<int> RectToDraw = Rect<int>(100, 100, 100, 100);
	Graphic().DrawRect(true, &RectToDraw, Color::Black);
}

void Core::FunctionalityComponent::DrawString() const
{
	float TextW = 1000.f;
	float TextH = 100.f;
	int WindowW = 0;
	int WindowH = 0;
	Graphic().GetWindowSize(&WindowW, &WindowH);
	Graphic().DrawString("Bonjour Jean-Sébastien, dirigez-vous vers la porte!", mFontId, (WindowW / 2) - (TextW / 2), (WindowH / 4) - (TextH / 2), TextW, TextH, Color::Red);
}

void Core::FunctionalityComponent::Destroy()
{
	Component::Destroy();
	
}
