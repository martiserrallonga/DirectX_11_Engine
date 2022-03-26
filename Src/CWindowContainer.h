#pragma once
#include "CRenderWindow.h"
#include "CKeyboard.h"
#include "CMouse.h"
#include "CGraphics.h"

class CWindowContainer
{
public:
	CWindowContainer();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CRenderWindow RenderWindow;
	CKeyboard Keyboard;
	CMouse Mouse;
	CGraphics Graphics;

};
