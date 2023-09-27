#include "Utility.h"

// Color
const Core::Color Core::Color::Black = { 0, 0, 0, 255 };
const Core::Color Core::Color::Blue = { 0, 0, 255, 255 };
const Core::Color Core::Color::Green = { 0, 255, 0, 255 };
const Core::Color Core::Color::Cyan = { 51, 153, 255, 0 };
const Core::Color Core::Color::Red = { 255, 0, 0, 255 };
const Core::Color Core::Color::Purple = { 102, 0, 204, 255 };
const Core::Color Core::Color::Yellow = { 255, 255, 0, 255 };
const Core::Color Core::Color::White = { 255, 255, 255, 255 };
const Core::Color Core::Color::Grey = { 192, 192, 192, 255 };

// Vector
const Core::Vector2D<float> Core::Vector2D<float>::ZeroVectorF = Vector2D(0.f, 0.f);
const Core::Vector2D<int> Core::Vector2D<int>::ZeroVector = Vector2D(0, 0);

// Flip
const Core::Flip Core::Flip::None = Core::Flip(false, false);
