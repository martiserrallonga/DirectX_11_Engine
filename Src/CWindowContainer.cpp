#include "CWindowContainer.h"

constexpr int WasCharPressedBit = 1 << 30;

LRESULT CWindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Keyboard Messages
	case WM_KEYDOWN:
	{
		auto keycode = static_cast<unsigned char>(wParam);
		if (Keyboard.IsAutoRepeatKeys() || !(lParam & WasCharPressedBit)) Keyboard.OnKeyPressed(keycode);
		break;
	}
	case WM_KEYUP:
	{
		auto keycode = static_cast<unsigned char>(wParam);
		Keyboard.OnKeyReleased(keycode);
		break;
	}
	case WM_CHAR:
	{
		auto ch = static_cast<unsigned char>(wParam);
		if (Keyboard.IsAutoRepeatChars() || !(lParam & WasCharPressedBit)) Keyboard.OnChar(ch);
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
