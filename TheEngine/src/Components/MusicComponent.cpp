#include "Components/MusicComponent.h"

Core::MusicComponent::MusicComponent(Object* Owner) :
	Component(Owner),
	mMusicFile("")
{
}

void Core::MusicComponent::Start()
{
	mMusicId = Audio().LoadMusic(mMusicFile.c_str());
	Audio().PlayMusic(mMusicId, true);
}

void Core::MusicComponent::SetMusicFile(const std::string& File)
{
	mMusicFile = File;
}
