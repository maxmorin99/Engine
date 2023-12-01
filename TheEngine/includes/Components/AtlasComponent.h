#pragma once

#include "SpriteComponent.h"
#include "Interfaces/IDrawable.h"
#include <unordered_map>

namespace Core
{
	struct Frame
	{
		Frame();
		Frame(int X, int Y, int W, int H, const std::string& InName);
		static Frame Empty();

		Rect<int> Rect;
		std::string Name;
	};

	class AtlasComponent : public SpriteComponent
	{
	public:
		AtlasComponent(Object* Owner);
		virtual ~AtlasComponent() = default;
		virtual void Start();
		virtual void Destroy();
		virtual void Draw() override;
		virtual Component* Clone(Object* Owner) override;
		virtual void SetupClone(Component* Child) override;

		/** Add a new frame in the map. This frame will be available to be render on demands */
		void AddFrame(const std::string& Name, int X, int Y, int W, int H);
		void AddFrame(Frame& InFrame);

		/** Set a specific frame to be rendered */
		void SetFrame(const std::string& Name);

	protected:
		Vector<int> mTextureSize = Vector<int>::ZeroVector();

		/** Map of all the avalable frames */
		std::unordered_map<std::string, Frame> mFrames;
		Frame mCurrentFrame = Frame::Empty();

		void SetFrame(Frame& InFrame);
	};
}