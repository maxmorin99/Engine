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
	Audio().SetMusicVolume(mMusicId, 0);
}

void Core::MusicComponent::SetMusicFile(const std::string& File)
{
	mMusicFile = File;
}

Core::Component* Core::MusicComponent::Clone(Object* Owner)
{
	MusicComponent* Clone = new MusicComponent(Owner);
	__super::SetupClone(Clone);
	return Clone;
}

void Core::MusicComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);
}
