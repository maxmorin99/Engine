#pragma once

#include <functional>

namespace Core
{
    enum class EKey 
    {
        A = 0,
        B = 1,
        C = 2,
        D = 3,
        E = 4,
        F = 5,
        G = 6,
        H = 7,
        I = 8,
        J = 9,
        K = 10,
        L = 11,
        M = 12,
        N = 13,
        O = 14,
        P = 15,
        Q = 16,
        R = 17,
        S = 18,
        T = 19,
        U = 20,
        V = 21,
        W = 22,
        X = 23,
        Y = 24,
        Z = 25,
        ESC = 26,
        Space = 27,
        Right = 28,
        Left = 29,
        Down = 30,
        Up = 31,
    };

	class IInput
	{
	public:
		virtual ~IInput() = default;

        /**
        * @brief Checks if a specific keyboard key is currently pressed.
        * @param Key The enumeration representing the keyboard key.
        */
		virtual bool IsKeyDown(const EKey& Key) = 0;

        /**
        * @brief Checks if a specific mouse button is currently pressed.
        * @param Button The index of the mouse button.
        */
		virtual bool IsButtonDown(int Button) = 0;

        /**
        * @brief Retrieves the current mouse position.
        * @param X A pointer to store the X-coordinate of the mouse position.
        * @param Y A pointer to store the Y-coordinate of the mouse position.
        */
		virtual void GetMousePosition(int* X, int* Y) = 0;

        /**
        * @brief Binds a function to be called when a quit event is detected.
        * @param FunPtr The function pointer to the quit event handling function.
        */
        virtual void BindQuitFunction(std::function<void()> FunPtr) = 0;

	protected:
		friend class Engine;

        /**
        * @brief Process inputs.
        */
		virtual void Update() = 0;
	};
}
