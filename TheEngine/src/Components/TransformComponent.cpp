#include "Components/TransformComponent.h"
#include "Object.h"

Core::TransformComponent::TransformComponent(Object* Owner) :
	Component(Owner)
{
}

Core::TransformComponent::TransformComponent(Object* Owner, const Vector<float>& Loc, const Vector<float>& Size, float Rotation):
	Component(Owner),
	mLocation(Loc),
	mSize(Size),
	mRotation(Rotation)
{
}

void Core::TransformComponent::Start()
{
}

void Core::TransformComponent::Destroy()
{
}

Core::Component* Core::TransformComponent::Clone(Object* Owner)
{
	TransformComponent* Clone = new TransformComponent(Owner);
	__super::SetupClone(Clone);

	Clone->SetLocation(GetLocation());
	Clone->SetSize(GetSize());
	Clone->SetRotation(0.f);
	return Clone;
}

void Core::TransformComponent::SetupClone(Component* Child)
{
	__super::SetupClone(Child);
}

void Core::TransformComponent::GetLocation(float* OutX, float* OutY)
{
	if (!OutX || !OutY) return;

	*OutX = mLocation.X;
	*OutY = mLocation.Y;
}

void Core::TransformComponent::SetLocation(float NewX, float NewY)
{
	mLocation.X = NewX;
	mLocation.Y = NewY;
}

void Core::TransformComponent::GetSize(float* OutW, float* OutH) const
{
	if (!OutW || !OutH) return;

	*OutW = mSize.X;
	*OutH = mSize.Y;
}

void Core::TransformComponent::SetSize(float NewW, float NewH)
{
	mSize.X = NewW;
	mSize.Y = NewH;
}

Core::Vector<float> Core::TransformComponent::GetForwardVector() const
{
	// Convert the rotation from degrees to radians
	float radians = (float)((mRotation * PI) / 180.0f);

	// Calculate the forward vector using trigonometry
	float x = cos(radians);
	float y = sin(radians);
	Vector<float> Fwd = Vector<float>(x, y).GetNormalized();

	return mOwner->GetFlip().H ? -Fwd : Fwd;
}

