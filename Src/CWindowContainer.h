#pragma once
#include "CRenderWindow.h"
#include "CKeyboard.h"
#include "CMouse.h"

class CWindowContainer
{
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CRenderWindow RenderWindow;
	CKeyboard Keyboard;
	CMouse Mouse;

};
