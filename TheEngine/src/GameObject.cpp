#include "GameObject.h"

void Core::GameObject::Init()
{
}

void Core::GameObject::Start()
{
}

void Core::GameObject::Update(float deltaTime)
{
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
