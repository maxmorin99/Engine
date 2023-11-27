#pragma once

#include "Component.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IUpdatable.h"

namespace Core
{
	/*
	*	This class is only used to show the functionality requested for delivery 1.
	*/
	class FunctionalityComponent : public Component, public IDrawable, public IUpdatable
	{
	public:
		FunctionalityComponent(Object* Owner);
		virtual ~FunctionalityComponent() = default;
		virtual void Start();
		virtual void Destroy();
		virtual void Draw() override;
		virtual void Update(float DeltaTime) override;
		virtual Component* Clone(Object* Owner);
		virtual void SetupClone(Component* Child);

		void SetMusicFile(const char* File, int Volume);
		void SetFontFile(const char* File, int Size);
		void SetSoundFile(const char* File, int Volume);

	private:
		size_t mMusicId = 0;
		size_t mFontId = 0;
		size_t mSoundId = 0;

		float mPlaySoundDelay = 1.f;
		float mDelay = 0.f;
		
		void PlayMusic() const;
		void PlaySfx() const;
		void DrawRect() const;
		void DrawString() const;
	};
}