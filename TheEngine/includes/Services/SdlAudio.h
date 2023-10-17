#pragma once

#include "Interfaces/IAudio.h"
#include <unordered_map>

typedef struct _Mix_Music Mix_Music;
struct Mix_Chunk;

namespace Core
{
	class SdlAudio final : public IAudio
	{
	public:
		virtual ~SdlAudio() = default;
		virtual void Init(const char** ErrorMsg = nullptr) override;
		size_t LoadMusic(const char* FileName) override;
		size_t LoadSound(const char* FileName) override;
		int PlayMusic(size_t Id) override;
		int PlayMusic(size_t Id, int Loop) override;
		int PlaySFX(size_t Id) override;
		int PlaySFX(size_t Id, int Loop) override;
		void PauseMusic() override;
		void StopMusic() override;
		void ResumeMusic() override;
		void SetVolume(int Volume) override;
		virtual void SetSFXVolume(size_t SoundId, int Volume) override;
		virtual void SetMusicVolume(size_t MusicId, int Volume) override;
		virtual void ShutDown() override;

	private:
		std::unordered_map<size_t, Mix_Music*> mMusicMap;
		std::unordered_map<size_t, Mix_Chunk*> mSoundMap;
	};
}