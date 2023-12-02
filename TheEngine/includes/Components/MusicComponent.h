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
		virtual Component* Clone(Object* Owner);
		virtual void SetupClone(Component* Child);

	private:
		std::string mMusicFile;
		size_t mMusicId = 0;

	public:
		/** Set the file associated with the music
		@param File: Path to the file
		*/
		void SetMusicFile(const std::string& File);

		/** Get the id associated with the music file */
		inline size_t GetMusicId() const { return mMusicId; }
	};
}