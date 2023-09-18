#pragma once

namespace Core
{
	class IInput;
	class ILogger;
	class IGraphic;
	class ITimer;

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
		static Engine* _Instance;
		bool _IsRunning = false;
		bool _IsInit = false;

		IInput* _Input = nullptr;
		ILogger* _Logger = nullptr;
		IGraphic* _Graphic = nullptr;
		ITimer* _Timer = nullptr;

	public:
		inline static IInput* Input() { return GetInstance()->_Input; }
		inline static ILogger* Logger() { return GetInstance()->_Logger; }
		inline static IGraphic* Graphic() { return GetInstance()->_Graphic; }
		inline static ITimer* Timer() { return GetInstance()->_Timer; }
	};
}