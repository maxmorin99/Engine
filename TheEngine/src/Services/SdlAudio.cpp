#include "Services/SdlAudio.h"
#include <SDL_mixer.h>
#include <string>

void Core::SdlAudio::Init(const char** ErrorMsg)
{
    *ErrorMsg = "Audio init was succesful\n";
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
    {
        *ErrorMsg = SDL_GetError();
    }
}

size_t Core::SdlAudio::LoadMusic(const char* FileName)
{
    size_t MusicId = std::hash<std::string>()(FileName);

    if (mMusicMap.find(MusicId) == mMusicMap.end())
    {
        Mix_Music* Music = Mix_LoadMUS(FileName);
        if (!Music) return -1;
        mMusicMap[MusicId] = Music;
    }

    return MusicId;
}

size_t Core::SdlAudio::LoadSound(const char* FileName)
{
    size_t SoundId = std::hash<std::string>()(FileName);

    if (mSoundMap.find(SoundId) == mSoundMap.end())
    {
        Mix_Chunk* Sound = Mix_LoadWAV(FileName);
        if (!Sound) return -1;
        mSoundMap[SoundId] = Sound;
    }

    return SoundId;
}

int Core::SdlAudio::PlayMusic(size_t Id)
{
    if (mMusicMap.find(Id) == mMusicMap.end()) return -1;
    Mix_Music* MusicToPlay = mMusicMap[Id];
    if (!MusicToPlay) return -1;
    return Mix_PlayMusic(MusicToPlay, 0);
}

int Core::SdlAudio::PlayMusic(size_t Id, int Loop)
{
    if (mMusicMap.find(Id) == mMusicMap.end()) return -1;
    Mix_Music* MusicToPlay = mMusicMap[Id];
    return Mix_PlayMusic(MusicToPlay, Loop);
}

int Core::SdlAudio::PlaySFX(size_t Id)
{
    if (mSoundMap.find(Id) == mSoundMap.end()) return -1;
    Mix_Chunk* SoundToPlay = mSoundMap[Id];
    return Mix_PlayChannel(-1, SoundToPlay, 0);
}

int Core::SdlAudio::PlaySFX(size_t Id, int Loop)
{
    if (mSoundMap.find(Id) == mSoundMap.end()) return -1;
    Mix_Chunk* SoundToPlay = mSoundMap[Id];
    return Mix_PlayChannel(-1, SoundToPlay, Loop);
}

void Core::SdlAudio::PauseMusic()
{
    if (Mix_PlayingMusic != 0)
    {
        Mix_PauseMusic();
    }
}

void Core::SdlAudio::StopMusic()
{
    if (Mix_PlayingMusic != 0)
    {
        Mix_HaltMusic();
    }
}

void Core::SdlAudio::ResumeMusic()
{
    if (Mix_PlayingMusic == 0)
    {
        Mix_ResumeMusic();
    }
}

void Core::SdlAudio::SetVolume(int Volume)
{
    Mix_Volume(-1, Volume);
}

void Core::SdlAudio::SetSFXVolume(size_t SoundId, int Volume)
{
    if (mSoundMap.find(SoundId) == mSoundMap.end()) return;
    Mix_Chunk* Sfx = mSoundMap[SoundId];
    Mix_VolumeChunk(Sfx, Volume);
}

void Core::SdlAudio::SetMusicVolume(size_t MusicId, int Volume)
{
    Mix_VolumeMusic(Volume);
}

void Core::SdlAudio::ShutDown()
{
    for (auto& Music : mMusicMap)
    {
        Mix_FreeMusic(Music.second);
        Music.second = nullptr;
    }
    for (auto& Sfx : mSoundMap)
    {
        Mix_FreeChunk(Sfx.second);
        Sfx.second = nullptr;
    }
    mMusicMap.clear();
    mSoundMap.clear();
    Mix_CloseAudio();
}
