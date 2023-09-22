#pragma once

namespace Core
{
	class IInput;
	class ILogger;
	class IGraphic;
	class ITimer;
	class World;


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
		void ProcessInput(void);
		void Update(float DeltaTime);
		void Render(void);
		void Shutdown(void);
		static Engine* GetInstance();

	private:
		static Engine* mInstance;
		bool mIsRunning = false;
		bool mIsInit = false;

		IInput* mInput = nullptr;
		ILogger* mLogger = nullptr;
		IGraphic* mGraphic = nullptr;
		ITimer* mTimer = nullptr;
		World* mWorld = nullptr;

	public:
		inline static IInput* GetInput() { return GetInstance()->mInput; }
		inline static ILogger* GetLogger() { return GetInstance()->mLogger; }
		inline static IGraphic* GetGraphic() { return GetInstance()->mGraphic; }
		inline static ITimer* GetTimer() { return GetInstance()->mTimer; }
		inline static World* GetWorld() { return GetInstance()->mWorld; }
	};
}