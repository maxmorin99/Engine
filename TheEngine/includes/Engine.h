#pragma once

#include <string>
#include "IInput.h"

class SDL_Renderer;
class SDL_Window;

namespace Core
{
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
		SDL_Renderer* _Renderer = nullptr;
		SDL_Window* _Window = nullptr;
		const uint8_t* _KeyStates = nullptr;

		IInput* _Input;

	public:
		inline IInput* GetInput() const { return _Input; }
	};
}