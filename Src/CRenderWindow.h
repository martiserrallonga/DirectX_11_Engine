#pragma once
#include <Windows.h>
#include <string>

class CInput;

class CRenderWindow
{
public:
	~CRenderWindow();
	
	bool Init(HINSTANCE hInstance, CInput& input);
	bool ProcessMessages();
	
	HWND GetHWND() const { return _hwnd; }
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

private:
	void RegisterWindowClass();
	
	HINSTANCE _hInstance = NULL;
	HWND _hwnd = NULL;

	int mWidth = 0;
	int mHeight = 0;

	// TODO delete
	std::string mTitle;
	std::wstring mWTitle;
	std::string mClass;
	std::wstring mWClass;
};
