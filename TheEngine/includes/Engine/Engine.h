#pragma once
#include "Interfaces/IInput.h"
#include "Interfaces/ILogger.h"
#include "Interfaces/IGraphic.h"
#include "Interfaces/ITime.h"
#include "Interfaces/IWorld.h"
#include "Interfaces/IAudio.h"
#include "Interfaces/ISpawner.h"

namespace Core
{

	class Engine final
	{
	public:
		~Engine() = default;
		/** Initialize all the services
		*	@param Name: Window Name
		*	@param Width: Window width
		*	@param Height: Window height
		*/
		static bool Init(const char* Name, int Width, int Height);

		/** Start the game. At this point, all the services are initialized */
		static void Start(void);

		/** Called from the input service when the esc key is pressed */
		static void QuitButtonPressed();

	private:
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		/** Update the input service */
		static void ProcessInput(void);

		/** Update the world service */
		static void Update(float DeltaTime);

		/** Clear the window and render the world service */
		static void Render(void);

		/** Shutdown services */
		static void Shutdown(void);

		/** Get the instance of the engine */
		static Engine* GetInstance();

		/** Called when WindowQuit is called */
		void WindowQuitCallback();

	private:
		static Engine* mInstance;
		bool mIsRunning = false;
		bool mIsInit = false;

		IInput* mInput = nullptr;
		ILogger* mLogger = nullptr;
		IGraphic* mGraphic = nullptr;
		ITimer* mTimer = nullptr;
		IWorld* mWorld = nullptr;
		IAudio* mAudio = nullptr;
		ISpawner* mSpawner = nullptr;

	public:
		inline static IInput& GetInput() { return *GetInstance()->mInput; }
		inline static ILogger& GetLogger() { return *GetInstance()->mLogger; }
		inline static IGraphic& GetGraphic() { return *GetInstance()->mGraphic; }
		inline static ITimer& GetTimer() { return *GetInstance()->mTimer; }
		inline static IWorld& GetWorld() { return *GetInstance()->mWorld; }
		inline static IAudio& GetAudio() { return *GetInstance()->mAudio; }
		inline static ISpawner& GetSpawner() { return *GetInstance()->mSpawner; }
	};
}