#pragma once

#include <string>

class SDL_Renderer;
class SDL_Window;

namespace Engine
{
	class Engine final
	{
	public:
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
		static SDL_Renderer* _Renderer;
		static SDL_Window* _Window;
		static const uint8_t* _KeyStates;
	};
}