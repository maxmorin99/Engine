#pragma once

namespace Core
{
	class IInput;
	class ILogger;
	class IGraphic;
	class ITimer;
	class IWorld;
	class IAudio;
	//class ITileMap;

	class Engine final
	{
	public:
		~Engine() = default;
		static bool Init(const char* Name, int Width, int Height);
		static void Start(void);

	private:
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;
		static void ProcessInput(void);
		static void Update(float DeltaTime);
		static void Render(void);
		static void Shutdown(void);
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
		//ITileMap* mTileMap = nullptr;

	public:
		inline static IInput& GetInput() { return *GetInstance()->mInput; }
		inline static ILogger& GetLogger() { return *GetInstance()->mLogger; }
		inline static IGraphic& GetGraphic() { return *GetInstance()->mGraphic; }
		inline static ITimer& GetTimer() { return *GetInstance()->mTimer; }
		inline static IWorld& GetWorld() { return *GetInstance()->mWorld; }
		inline static IAudio& GetAudio() { return *GetInstance()->mAudio; }
	};
}