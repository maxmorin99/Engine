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

		/** Add a new frame from a sprite sheet or single image. This frame will be available to be render on demands
		*	@param X: if from a sprite sheet, represents the index from left to right. If from a single image, 0 will be on the image's top left
		*	@param Y: if from a sprite sheet, represents the index from top to bottom. If from a single image, 0 will be on the image's top left
		*	@param W: Horizontal size of the image
		*	@param W: Vertical size of the image
		*/
		void AddFrame(const std::string& Name, int X, int Y, int W, int H);

		/** Add a new frame from a sprite sheet or single image. This frame will be available to be render on demands
		*	@param InFrame: Frame to add to the list
		*/
		void AddFrame(Frame& InFrame);

		/** Set a specific frame to be rendered
		*	@param Name: The name of the frame to set
		*/
		void SetFrame(const std::string& Name);

	protected:
		Vector<int> mTextureSize = Vector<int>::ZeroVector();
		std::unordered_map<std::string, Frame> mFrames;
		Frame mCurrentFrame = Frame::Empty();

		/** Set a specific frame to be rendered
		*	@param InFrame: Reference to the frame to set
		*/
		void SetFrame(Frame& InFrame);
	};
}