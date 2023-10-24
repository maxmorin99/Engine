#include "Components/TargetCursorComponent.h"
#include "Object.h"

TargetCursorComponent::TargetCursorComponent(Object* Owner) :
	SpriteComponent(Owner)
{
}

void TargetCursorComponent::Update(float DeltaTime)
{
	Vector<int> MousePos = Vector<int>::ZeroVector();
	Input().GetMousePosition(&MousePos.X, &MousePos.Y);
	Vector<float> MousePosF = Vector<float>(static_cast<float>(MousePos.X), static_cast<float>(MousePos.Y));

	// place the sprite in the mouse center pos
	float NewTargetX = MousePosF.X - mOwner->GetSize().X / 2;
	float NewTargetY = MousePosF.Y - mOwner->GetSize().Y / 2;
	mOwner->SetLocation(NewTargetX, NewTargetY);
}

void TargetCursorComponent::Draw()
{
	SpriteComponent::Draw();
}
