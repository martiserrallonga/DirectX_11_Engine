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

	while (!Mouse.IsEventBufferEmpty()) {
		CMouseEvent Event = Mouse.ReadEvent();
		auto pos = Event.GetPos();
		std::string msg = "Mouse: [";
		msg += std::to_string(pos.x);
		msg += ' ';
		msg += std::to_string(pos.y);
		msg += "]";

		if (Event.GetType() == CMouseEvent::EEventType::RawMove) msg += " Raw";
		if (Event.GetType() == CMouseEvent::EEventType::RPress) msg += " RPress";
		if (Event.GetType() == CMouseEvent::EEventType::RRelease) msg += " RRelease";
		if (Event.GetType() == CMouseEvent::EEventType::LPress) msg += " LPress";
		if (Event.GetType() == CMouseEvent::EEventType::LRelease) msg += " LRelease";
		if (Event.GetType() == CMouseEvent::EEventType::MPress) msg += " MPress";
		if (Event.GetType() == CMouseEvent::EEventType::MRelease) msg += " MRelease";
		if (Event.GetType() == CMouseEvent::EEventType::WheelUp) msg += " WheelUp";
		if (Event.GetType() == CMouseEvent::EEventType::WheelDown) msg += " WheelDown";

		msg += '\n';
		OutputDebugStringA(msg.c_str());
	}

}
