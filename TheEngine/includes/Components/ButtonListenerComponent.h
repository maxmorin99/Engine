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
		
		void Play();
		void Quit();

		// Hérité via IObserver
		void OnNotify(const std::unordered_map<std::string, void*>& Value) override;
	};
}