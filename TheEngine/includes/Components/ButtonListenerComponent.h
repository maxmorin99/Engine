#pragma once

#include "Component.h"
#include "Interfaces/IObserver.h"
#include <string>
#include <unordered_map>

namespace Core
{
	class ButtonListenerComponent : public Component, public IObserver<std::unordered_map<std::string, void*>>
	{
	public:
		ButtonListenerComponent(Object* Owner);
		virtual ~ButtonListenerComponent() = default;

		/** Called when the play button is pressed */
		void Play();

		/** Called when the quit button is pressed */
		void Quit();

		/** Called when the listened button is clicked */
		void OnNotify(const std::unordered_map<std::string, void*>& Value) override;
	};
}