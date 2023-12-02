#pragma once

namespace Core
{
	class IDrawable
	{
	public:
		/**
		* @brief Render on to the screen.
		*/
		virtual void Draw() = 0;
	};
}