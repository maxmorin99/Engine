#pragma once

namespace Core
{
    enum class EKey {
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
    };

	class IInput
	{
	public:
		virtual ~IInput() = default;
		virtual bool IsKeyDown(const EKey& Key) = 0;
		virtual bool IsButtonDown(int Button) = 0;
		virtual void GetMousePosition(int* X, int* Y) = 0;
		virtual bool ShouldQuit() = 0;

	protected:
		friend class Engine;
		virtual void Update() = 0;
	};
}
