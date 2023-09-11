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
		Engine() = default;
		~Engine() = default;
		bool Init(const char* Name, int Width, int Height);
		void Start(void);

	private:
		void ProcessInput(void);
		void Update(float DeltaTime);
		void Render(void);
		void Shutdown(void);

	private:
		bool _IsRunning = false;
		bool _IsInit = false;

		IInput* _Input = nullptr;
		ILogger* _Logger = nullptr;
		IGraphic* _Graphic = nullptr;
		ITimer* _Timer = nullptr;

	public:
		inline IInput* GetInput() const { return _Input; }
		inline ILogger* GetLogger() const { return _Logger; }
		inline IGraphic* GetGraphic() const { return _Graphic; }
		inline ITimer* GetTimer() const { return _Timer; }
	};
}