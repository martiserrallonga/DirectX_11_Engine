#include "CInput.h"
#include "CErrorLogger.h"
#include <ImGui/imgui.h>
#include <memory>

static constexpr const unsigned kWasCharPressedBit = 1 << 30;

CInput::CInput()
{
	RegisterMouse();
}

LRESULT CInput::HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		const auto pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		auto input = std::_Bit_cast<CInput*>(pCreate->lpCreateParams);
		if (!input) {
			CErrorLogger::Log("Critical Error: Pointer to window container is nullptr during WM_NCCREATE.");
			exit(-1);
		}
		OutputDebugStringA("The window was created.\n");

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(input));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(CInput::HandleMessageRedirect));
		return input->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CInput::HandleMessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE) {
		DestroyWindow(hwnd);
		return 0;
	}

	auto input = std::_Bit_cast<CInput*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return input->WindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CInput::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) return true;

	switch (uMsg)
	{
		// Keyboard Messages
	case WM_KEYDOWN:
	{
		auto keycode = static_cast<unsigned char>(wParam);
		if (Keyboard.IsAutoRepeatKeys() || !(lParam & kWasCharPressedBit)) Keyboard.OnKeyPressed(keycode);
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
		if (Keyboard.IsAutoRepeatChars() || !(lParam & kWasCharPressedBit)) Keyboard.OnChar(ch);
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
	// Mouse Raw Input Messages
	case WM_INPUT:
	{
		ProcRawInputData(lParam);
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void CInput::RegisterMouse()
{
	static bool RawInputInitialized = false;
	if (RawInputInitialized) return;

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = NULL;

	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		CErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
		exit(-1);
	}

	RawInputInitialized = true;
}

void CInput::ProcRawInputData(LPARAM lParam)
{
	UINT DataSize = 0U;
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
}
