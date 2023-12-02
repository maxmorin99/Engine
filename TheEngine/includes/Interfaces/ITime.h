#pragma once

namespace Core
{
	class ITimer
	{
	public:
		virtual ~ITimer() = default;

		/**
		* @brief Starts the timer.
		*/
		virtual void StartTimer() = 0;

		/**
		* @brief Updates the start time for measuring delta time.
		*/
		virtual void UpdateStart() = 0;

		/**
		* @brief Updates the end time for measuring delta time.
		*/
		virtual void UpdateEnd() = 0;

		/**
		* @brief Sets the target frames per second (FPS) for the timer.
		* @param Fps The target frames per second.
		*/
		virtual void SetTargetFps(const float Fps) = 0;

		/**
		* @brief Gets the delta time between the last two updates.
		*/
		virtual float GetDeltaTime() const = 0;
	};
}