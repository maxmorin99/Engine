#pragma once

namespace Core
{
	class IAudio
	{
	public:
		virtual ~IAudio() = default;

		/**
		* @brief Initializes the audio service.
		* @param ErrorMsg A pointer to a char pointer where any error message can be stored.
		*/
		virtual void Init(const char** ErrorMsg = nullptr) = 0;

		/**
		* @brief Loads a music file.
		* @param FileName The name of the music file to load.
		*/
		virtual size_t LoadMusic(const char* FileName) = 0;

		/**
		* @brief Loads a sound file.
		* @param FileName The name of the sound file to load.
		*/
		virtual size_t LoadSound(const char* FileName) = 0;

		/**
		* @brief Plays the music with the specified ID.
		* @param Id The ID of the music to play.
		*/
		virtual int PlayMusic(size_t Id) = 0;

		/**
		* @brief Plays the music with the specified ID and loop setting.
		* @param Id The ID of the music to play.
		* @param Loop If set to 1, the music will loop; otherwise, it plays once.
		*/
		virtual int PlayMusic(size_t Id, int Loop) = 0;

		/**
		* @brief Plays the sound effect with the specified ID.
		* @param Id The ID of the sound effect to play.
		*/
		virtual int PlaySFX(size_t Id) = 0;

		/**
		* @brief Plays the sound effect with the specified ID and loop setting.
		* @param Id The ID of the sound effect to play.
		* @param Loop If set to 1, the sound effect will loop; otherwise, it plays once.
		*/
		virtual int PlaySFX(size_t Id, int Loop) = 0;

		/**
		* @brief Pauses the currently playing music.
		*/
		virtual void PauseMusic() = 0;

		/**
		* @brief Stops the currently playing music.
		*/
		virtual void StopMusic() = 0;

		/**
		* @brief Resumes the paused music.
		*/
		virtual void ResumeMusic() = 0;

		/**
		* @brief Sets the overall volume of the audio service.
		* @param Volume The desired volume level (0 to 100).
		*/
		virtual void SetVolume(int Volume) = 0;

		/**
		* @brief Sets the volume of a specific sound effect.
		* @param SoundId The ID of the sound effect.
		* @param Volume The desired volume level (0 to 100).
		*/
		virtual void SetSFXVolume(size_t SoundId, int Volume) = 0;

		/**
		* @brief Sets the volume of a specific music track.
		* @param MusicId The ID of the music track.
		* @param Volume The desired volume level (0 to 100).
		*/
		virtual void SetMusicVolume(size_t MusicId, int Volume) = 0;

		/**
		* @brief Shuts down the audio service, releasing any allocated resources.
		*/
		virtual void ShutDown() = 0;
	};
}