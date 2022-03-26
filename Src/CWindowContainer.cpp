#include "CWindowContainer.h"
#include <memory>

constexpr int WasCharPressedBit = 1 << 30;

CWindowContainer::CWindowContainer()
{
	static bool RawInputInitialized = false;
	if (RawInputInitialized) return;
	
	RAWINPUTDEVICE Rid;
	Rid.usUsagePage = 0x01;
	Rid.usUsage = 0x02;
	Rid.dwFlags = 0;
	Rid.hwndTarget = NULL;

	if (RegisterRawInputDevices(&Rid, 1, sizeof(Rid)) == FALSE)
	{
		CErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
		exit(-1);
	}

	RawInputInitialized = true;
}

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
	case WM_INPUT:
	{
		UINT DataSize;
 		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &DataSize, sizeof(RAWINPUTHEADER));

		if (DataSize > 0) {
			auto RawData = std::make_unique<BYTE[]>(DataSize);
			if (DataSize == GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, RawData.get(), &DataSize, sizeof(RAWINPUTHEADER))) {
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(RawData.get());
				if (raw->header.dwType == RIM_TYPEMOUSE) {
					Mouse.OnMouseRawMove({ raw->data.mouse.lLastX, raw->data.mouse.lLastY });
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
