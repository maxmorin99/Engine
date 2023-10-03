#include "Components/Component.h"
#include "Engine/Engine.h"

Core::Component::Component(Object* Owner) :
	mOwner(Owner)
{
}

Core::IInput& Core::Component::Input() const
{
	return Engine::GetInput();
}

Core::IGraphic& Core::Component::Graphic() const
{
	return Engine::GetGraphic();
}

Core::ITimer& Core::Component::Timer() const
{
	return Engine::GetTimer();
}

Core::IAudio& Core::Component::Audio() const
{
	return Engine::GetAudio();
}

Core::ILogger& Core::Component::Logger() const
{
	return Engine::GetLogger();
}
