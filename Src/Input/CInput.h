#pragma once
#include "CKeyboard.h"
#include "CMouse.h"
#include <Windows.h>

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // ImGui Forward Declaration

class CInput {
public:
	CInput();

	static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void RegisterMouse();
	void ProcRawInputData(LPARAM lParam);

	CKeyboard Keyboard;
	CMouse Mouse;

	friend class CEngine;
};
