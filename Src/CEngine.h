#pragma once
#include "CWindowContainer.h"
#include "CTimer.h"

class CEngine : public CWindowContainer
{
public:
	bool Init(HINSTANCE aInstance, std::string aTitle, std::string aClass, int width, int height);
	bool ProcessMessages();
	void Update();
	void Render();

private:
	CTimer Timer;

};