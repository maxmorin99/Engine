#pragma once

#include "SpriteComponent.h"
#include "Interfaces/IDrawable.h"
#include <unordered_map>

namespace Core
{
	class AtlasComponent : public SpriteComponent
	{
	public:
		AtlasComponent(Object* Owner);
		virtual ~AtlasComponent() = default;
		virtual void Start();
		virtual void Destroy();
		virtual void Draw() override;

		void AddFrame(const std::string& Name, int X, int Y, int W, int H);
		void SetFrame(const std::string& Name);

	protected:
		Vector<int> mTextureSize = Vector<int>::ZeroVector();
		std::unordered_map<std::string, Rect<int>> mFrames;	
	};
}