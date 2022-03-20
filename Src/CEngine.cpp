#include "CEngine.h"

bool CEngine::Init(HINSTANCE aInstance, std::string aTitle, std::string aClass, int width, int height)
{
	return RenderWindow.Init(this, aInstance, aTitle, aClass, width, height);
}

bool CEngine::ProcessMessages()
{
	return RenderWindow.ProcessMessages();
}

void CEngine::Update()
{
	while (!Keyboard.IsCharBufferEmpty()) {
		unsigned char ch = Keyboard.ReadChar();
		
		// Test
		std::string msg = "Char: ";
		msg += ch;
		msg += '\n';
		OutputDebugStringA(msg.c_str());
	}

	while (!Keyboard.IsKeyBufferEmpty()) {
		CKeyboardEvent Event = Keyboard.ReadKey();
		unsigned char keycode = Event.GetKeyCode();

		// Test
		std::string msg;
		if (Event.IsPress()) msg = "Keydownup: ";
		if (Event.IsRelease()) msg = "Keyup: ";
		msg += keycode;
		msg += '\n';
		OutputDebugStringA(msg.c_str());
	}

}
