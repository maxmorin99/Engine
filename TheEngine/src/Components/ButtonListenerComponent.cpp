#include "Components/ButtonListenerComponent.h"
#include "Engine/Engine.h"

Core::ButtonListenerComponent::ButtonListenerComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::ButtonListenerComponent::Play()
{
	World().Load("FirstScene");
}

void Core::ButtonListenerComponent::Quit()
{
	Engine::QuitButtonPressed();
}

void Core::ButtonListenerComponent::OnNotify(const std::unordered_map<std::string, void*>& Value)
{
	void* ButtonNameVoidPtr = nullptr;
	if (Value.count("ButtonName") > 0)
	{
		ButtonNameVoidPtr = Value.at("ButtonName");
		std::string ButtonName = *(static_cast<std::string*>(ButtonNameVoidPtr));
		if (ButtonName == "Play")
		{
			Play();
		}
		else if (ButtonName == "Quit")
		{
			Quit();
		}
	}
}
