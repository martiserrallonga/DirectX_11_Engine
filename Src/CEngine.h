#pragma once
#include "CInput.h"
#include "CWindow.h"
#include "CGraphics.h"
#include "CTimer.h"
#include <json.h>

class CEngine
{
public:
	bool Init(std::string configPath, HINSTANCE aInstance);
	bool ProcessMessages();
	void Update();
	void Render();

private:
	CInput Input;
	CWindow Window;
	CGraphics Graphics;
	CTimer Timer;

	std::string _configPath;
};
