#pragma once

namespace Core
{
	class IAudio
	{
	public:
		virtual void Init(const char** ErrorMsg = nullptr) = 0;
		virtual size_t LoadMusic(const char* FileName) = 0;
		virtual size_t LoadSound(const char* FileName) = 0;
		virtual int PlayMusic(size_t Id) = 0;
		virtual int PlayMusic(size_t Id, int Loop) = 0;
		virtual int PlaySFX(size_t Id) = 0;
		virtual int PlaySFX(size_t Id, int Loop) = 0;
		virtual void PauseMusic() = 0;
		virtual void StopMusic() = 0;
		virtual void ResumeMusic() = 0;
		virtual void SetVolume(int Volume) = 0;
		virtual void SetSFXVolume(size_t SoundId, int Volume) = 0;
		virtual void SetMusicVolume(size_t MusicId, int Volume) = 0;
		virtual void ShutDown() = 0;
	};
}