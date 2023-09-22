#include "GameObject.h"

void Core::GameObject::Start()
{
	GetLogger()->DebugLog(ConsoleColor::Red, "Start!\n");
}

void Core::GameObject::Update(float deltaTime)
{
	if (GetInput()->IsKeyDown(EKey::Down))
	{
		GetLogger()->DebugLog(ConsoleColor::Red, "Down key pressed!\n");
	}
}

void Core::GameObject::Destroy()
{
}

void Core::GameObject::GetObjectPosition(Vector2D<float>& OutLocation) const
{
	OutLocation = mLocation;
}

void Core::GameObject::GetObjectSize(float* OutW, float* OutH) const
{
	if (!OutW || !OutH) return;
	*OutW = mW;
	*OutH = mH;
}
