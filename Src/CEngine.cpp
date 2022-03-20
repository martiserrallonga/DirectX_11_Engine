#include "CEngine.h"

bool CEngine::Init(HINSTANCE aInstance, std::string aTitle, std::string aClass, int width, int height)
{
	return RenderWindow.Init(this, aInstance, aTitle, aClass, width, height);
}

bool CEngine::Update()
{
	return RenderWindow.Update();
}
