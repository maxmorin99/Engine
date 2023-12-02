#pragma once

#include "Utility.h"

namespace Core
{
	class IGraphic
	{
	public:
		virtual ~IGraphic() = default;

		/**
		* @brief Initializes the graphics system.
		* @param ErrorMsg A pointer to a char pointer where any error message can be stored.
		*/
		virtual bool Init(const char** ErrorMsg = nullptr) = 0;

		/**
		* @brief Initializes the graphics window.
		* @param ErrorMsg A pointer to a char pointer where any error message can be stored.
		*/
		virtual bool InitWindow(const char** ErrorMsg = nullptr) = 0;

		/**
		* @brief Initializes the graphics renderer.
		* @param ErrorMsg A pointer to a char pointer where any error message can be stored.
		*/
		virtual bool InitRenderer(const char** ErrorMsg = nullptr) = 0;

		/**
		* @brief Sets the draw color for subsequent drawing operations.
		* @param NewColor The new color to set.
		*/
		virtual void SetDrawColor(const Color& NewColor) = 0;

		/**
		* @brief Clears the rendering surface.
		*/
		virtual void Clear() = 0;

		/**
		* @brief Presents images/shapes to the screen.
		*/
		virtual void Present() = 0;

		/**
		* @brief Draws a rectangle on the screen.
		* @param bFill If true, the rectangle is filled; otherwise, it's just outlined.
		* @param RectX The X-coordinate of the rectangle.
		* @param RectY The Y-coordinate of the rectangle.
		* @param RectW The width of the rectangle.
		* @param RectH The height of the rectangle.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawRect(bool bFill, const int RectX, const int RectY, const int RectW, const int RectH, const Color& DrawColor) = 0;

		/**
		* @brief Draws a rectangle on the screen using a Rect<int> object.
		* @param bFill If true, the rectangle is filled; otherwise, it's just outlined.
		* @param Rect A Rect<int> object specifying the rectangle's position and size.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawRect(bool bFill, Rect<int>* Rect, const Color& DrawColor) = 0;

		/**
		* @brief Draws a filled rectangle on the screen using floating-point coordinates.
		* @param bFill If true, the rectangle is filled; otherwise, it's just outlined.
		* @param RectX The X-coordinate of the rectangle.
		* @param RectY The Y-coordinate of the rectangle.
		* @param RectW The width of the rectangle.
		* @param RectH The height of the rectangle.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawRectF(bool bFill, const float RectX, const float RectY, const float RectW, const float RectH, const Color& DrawColor) = 0;

		/**
		* @brief Draws a filled rectangle on the screen using a Rect<float> object.
		* @param bFill If true, the rectangle is filled; otherwise, it's just outlined.
		* @param Rect A Rect<float> object specifying the rectangle's position and size.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawRectF(bool bFill, Rect<float>* Rect, const Color& DrawColor) = 0;

		/**
		* @brief Draws a line on the screen using integer coordinates.
		* @param X1 The X-coordinate of the starting point.
		* @param Y1 The Y-coordinate of the starting point.
		* @param X2 The X-coordinate of the ending point.
		* @param Y2 The Y-coordinate of the ending point.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawLine(const int X1, const int Y1, const int X2, const int Y2, const Color& DrawColor) = 0;

		/**
		* @brief Draws a line on the screen using Vector<int> coordinates.
		* @param Point1 The starting point represented as a Vector<int>.
		* @param Point2 The ending point represented as a Vector<int>.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawLine(const Vector<int>& Point1, const Vector<int>& Point2, const Color& DrawColor) = 0;

		/**
		* @brief Draws a line on the screen using floating-point coordinates.
		* @param X1 The X-coordinate of the starting point.
		* @param Y1 The Y-coordinate of the starting point.
		* @param X2 The X-coordinate of the ending point.
		* @param Y2 The Y-coordinate of the ending point.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawLineF(const float X1, const float Y1, const float X2, const float Y2, const Color& DrawColor) = 0;

		/**
		* @brief Draws a line on the screen using Vector<float> coordinates.
		* @param Point1 The starting point represented as a Vector<float>.
		* @param Point2 The ending point represented as a Vector<float>.
		* @param DrawColor The color to use for drawing.
		*/
		virtual void DrawLineF(const Vector<float>& Point1, const Vector<float>& Point2, const Color& DrawColor) = 0;

		/**
		* @brief Draws a single point on the screen using floating-point coordinates.
		* @param Point The coordinates of the point represented as a Vector<float>.
		* @param Color The color to use for drawing the point.
		*/
		virtual void DrawPoint(const Vector<float>& Point, const Color& Color) = 0;

		/**
		* @brief Loads a texture from a file.
		* @param FileName The name of the file containing the texture.
		*/
		virtual size_t LoadTexture(const char* FileName) = 0;

		/**
		* @brief Draws a textured rectangle on the screen.
		* @param Id The ID of the loaded texture.
		* @param Src The source rectangle specifying which part of the texture to use.
		* @param Dst The destination rectangle specifying where to draw the texture on the screen.
		* @param Angle The angle of rotation for the texture.
		* @param TextureFlip The flipping settings for the texture.
		* @param Color The color to use for drawing the texture.
		*/
		virtual void DrawTexture(size_t Id, const Rect<int>& Src, const Rect<int>& Dst, double Angle, const Flip& TextureFlip, const Color& Color) = 0;
			
			/**
		* @brief Draws a textured rectangle on the screen with a fixed color.
		* @param Id The ID of the loaded texture.
		* @param Dst The destination rectangle specifying where to draw the texture on the screen.
		* @param Color The color to use for drawing the texture.
		*/
		virtual void DrawTexture(size_t Id, const Rect<int>& Dst, const Color& Color) = 0;

		/**
		* @brief Retrieves the size of a loaded texture.
		* @param Id The ID of the loaded texture.
		* @param OutW A pointer to store the width of the texture.
		* @param OutH A pointer to store the height of the texture.
		*/
		virtual void GetTextureSize(size_t Id, int* OutW, int* OutH) = 0;

		/**
		* @brief Loads a font from a file with a specified font size.
		* @param FileName The name of the file containing the font.
		* @param FontSize The size of the font.
		*/
		virtual size_t LoadFont(const char* FileName, int FontSize) = 0;

		/**
		* @brief Draws a string of text on the screen using a specified font.
		* @param Text The text to draw.
		* @param FontId The ID of the loaded font.
		* @param X The X-coordinate of the top-left corner of the text.
		* @param Y The Y-coordinate of the top-left corner of the text.
		* @param W The width of the text area.
		* @param H The height of the text area.
		* @param DrawColor The color to use for drawing the text.
		*/
		virtual void DrawString(const char* Text, size_t FontId, float X, float Y, float W, float H, const Color& DrawColor) = 0;

		/**
		* @brief Retrieves the size of a string of text using a specified font.
		* @param Text The text to measure.
		* @param FontId The ID of the loaded font.
		* @param OutW A pointer to store the width of the text.
		* @param OutH A pointer to store the height of the text.
		*/
		virtual void GetTextSize(const char* Text, size_t FontId, int* OutW, int* OutH) = 0;

		/**
		* @brief Retrieves the size of the graphics window.
		* @param WinW A pointer to store the width of the window.
		* @param WinH A pointer to store the height of the window.
		*/
		virtual void GetWindowSize(int* WinW, int* WinH) = 0;

		/**
		* @brief Shuts down the graphics system, releasing any allocated resources.
		*/
		virtual void ShutDown() = 0;
	};

}
