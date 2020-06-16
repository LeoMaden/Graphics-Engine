#pragma once

namespace Engine {

	enum class KeyCode
	{
		None = 0,

		// Main keyboard
		A, B, C, D, E,
		F, G, H, I, J,
		K, L, M, N, O,
		P, Q, R, S, T,
		U, V, W, X, Y, Z,

		LShift, LCtrl, Enter, Backspace,
		Escape, Spacebar,

		// Num row
		D0, D1, D2, D3, D4,
		D5, D6, D7, D8, D9,

		// Mouse
		Mouse1, Mouse2, Mouse3,
		Mouse4, Mouse5, Mouse6,

		// D-Pad
		Up, Down, Left, Right,

		// Keypad
		KP0, KP1, KP2, KP3, KP4,
		KP5, KP6, KP7, KP8, KP9

	};

	struct ModifierKeys
	{
		bool Ctrl = false;
		bool Shift = false;
		bool Mouse1 = false;
		bool Mouse2 = false;
		bool Mouse3 = false;
	};

}