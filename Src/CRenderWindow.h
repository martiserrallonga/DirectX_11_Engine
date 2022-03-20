#pragma once
#include "CErrorLogger.h"

class CWindowContainer;

class CRenderWindow
{
public:
	~CRenderWindow();
	
	bool Init(CWindowContainer* aWindowContainer, HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int width, int height);
	bool ProcessMessages();

private:
	void RegisterWindowClass() const;
	
	HWND Handle = NULL;						// Handle to this Window
	HINSTANCE hInstance = NULL;		// Handle to the Application instance
	std::string mTitle;
	std::wstring mWTitle;
	std::string mClass;
	std::wstring mWClass;
	int mWidth = 0;
	int mHeight = 0;

};

