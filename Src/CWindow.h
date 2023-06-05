#pragma once
#include <Windows.h>
#include <string>

class CInput;

class CWindow
{
public:
	~CWindow();
	
	bool Init(std::string configPath, HINSTANCE hInstance, CInput& input);
	bool ProcessMessages();
	
	HWND GetHWND() const { return _hwnd; }
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

private:
	void RegisterWindowClass();
	
	HINSTANCE _hInstance = NULL;
	HWND _hwnd = NULL;
	
	std::wstring mWTitle;
	std::wstring mWClass;
	int mWidth = 0;
	int mHeight = 0;
	
	std::string _configPath;
};
