#pragma once

#include "Component.h"

namespace Core
{
	class MusicComponent : public Component
	{
	public:
		MusicComponent(Object* Owner);
		virtual ~MusicComponent() = default;
		virtual void Start();
		void SetMusicFile(const std::string& File);

	private:
		std::string mMusicFile;
		size_t mMusicId = 0;
	};
}