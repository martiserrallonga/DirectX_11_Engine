#pragma once
#include "CInput.h"
#include "CRenderWindow.h"
#include "CGraphics.h"
#include "CTimer.h"

class CEngine
{
public:
	bool Init(HINSTANCE aInstance);
	bool ProcessMessages();
	void Update();
	void Render();

private:
	CInput Input;
	CRenderWindow RenderWindow; // TODO: Rename to CWindow Window
	CGraphics Graphics;
	CTimer Timer;
};
