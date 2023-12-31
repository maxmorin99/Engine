#include "Components/TargetCursorComponent.h"
#include "Object.h"
#include "Components/BoxComponent.h"

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
	BoxComponent* Box = mOwner->GetComponent<BoxComponent>();
	Box->SetCollisionLocation(Vector<float>(NewTargetX, NewTargetY));
}

void TargetCursorComponent::Draw()
{
	SpriteComponent::Draw();
}

Component* TargetCursorComponent::Clone(Object* Owner)
{
	TargetCursorComponent* Clone = new TargetCursorComponent(Owner);
	__super::SetupClone(Clone);

	return Clone;
}

void TargetCursorComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);
}
