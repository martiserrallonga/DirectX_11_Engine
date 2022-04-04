#include "CEngine.h"

bool CEngine::Init(HINSTANCE aInstance, std::string aTitle, std::string aClass, int width, int height)
{
	if (!RenderWindow.Init(this, aInstance, aTitle, aClass, width, height)) return false;
	if (!Graphics.Init(RenderWindow.GetHWND(), width, height)) return false;

	return true;
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


	CCamera& Camera = Graphics.Camera;
	const float CameraSpeed = 0.01f;

	while (!Mouse.IsEventBufferEmpty()) {
		CMouseEvent Event = Mouse.ReadEvent();
		if (Mouse.IsRightPressed()
			&& Event.GetType() == CMouseEvent::EEventType::RawMove)
		{
			auto pos = Event.GetPos();
			float pitch = static_cast<float>(pos.y) * DirectX::XM_PI / 180.f;
			float yaw = static_cast<float>(pos.x) * DirectX::XM_PI / 180.f;
			Camera.AddRotation({ pitch, yaw, 0.f });
		}
	}

	if (Keyboard.IsKeyPressed('W')) {
		Camera.AddPosition(+Camera.GetForward() * CameraSpeed);
	}
	if (Keyboard.IsKeyPressed('S')) {
		Camera.AddPosition(-Camera.GetForward() * CameraSpeed);
	}
	if (Keyboard.IsKeyPressed('A')) {
		Camera.AddPosition(-Camera.GetRight() * CameraSpeed);
	}
	if (Keyboard.IsKeyPressed('D')) {
		Camera.AddPosition(+Camera.GetRight() * CameraSpeed);
	}
	if (Keyboard.IsKeyPressed(VK_SPACE)) {
		Camera.AddPosition({ 0.f, +CameraSpeed, 0.f });
	}
	if (Keyboard.IsKeyPressed(VK_SHIFT)) {
		Camera.AddPosition({ 0.f, -CameraSpeed, 0.f });
	}

}

void CEngine::Render()
{
	Graphics.Render();
}
