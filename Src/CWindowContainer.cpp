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
	// Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse.OnMouseMove({ x, y });
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse.OnLeftPressed({ x, y });
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse.OnLeftReleased({ x, y });
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse.OnRightPressed({ x, y });
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse.OnRightReleased({ x, y });
		break;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse.OnMiddlePressed({ x, y });
		break;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse.OnMiddleReleased({ x, y });
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		GET_WHEEL_DELTA_WPARAM(wParam) > 0
			? Mouse.OnWheelUp({ x,y })
			: Mouse.OnWheelDown({ x,y });
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
