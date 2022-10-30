#include "CEngine.h"

bool CEngine::Init(HINSTANCE aInstance, std::string aTitle, std::string aClass, int width, int height)
{
	Timer.Start();

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
	float dt = static_cast<float>(Timer.GetMiliseconds());
	Timer.Restart();

	while (!Keyboard.IsCharBufferEmpty()) {
		unsigned char ch = Keyboard.ReadChar();
	}

	while (!Keyboard.IsKeyBufferEmpty()) {
		CKeyboardEvent Event = Keyboard.ReadKey();
	}

	// Input Move Camera 
	CCamera& Camera = Graphics.Camera;
	const float dx = Camera.Speed / 1000.f * dt;

	while (!Mouse.IsEventBufferEmpty()) {
		CMouseEvent Event = Mouse.ReadEvent();
		if (Mouse.IsRightPressed()
			&& Event.GetType() == CMouseEvent::EEventType::RawMove)
		{
			auto pos = Event.GetPos();
			float pitch = static_cast<float>(pos.y) * DirectX::XM_PI / 180.f;
			float yaw = static_cast<float>(pos.x) * DirectX::XM_PI / 180.f;
			Camera.AddRotation(pitch, yaw, 0.f);
		}
	}

	if (Keyboard.IsKeyPressed('W')) {
		Camera.AddPosition(+Camera.GetForward() * dx);
	}
	if (Keyboard.IsKeyPressed('S')) {
		Camera.AddPosition(-Camera.GetForward() * dx);
	}
	if (Keyboard.IsKeyPressed('A')) {
		Camera.AddPosition(-Camera.GetRight() * dx);
	}
	if (Keyboard.IsKeyPressed('D')) {
		Camera.AddPosition(+Camera.GetRight() * dx);
	}
	if (Keyboard.IsKeyPressed(VK_SPACE)) {
		Camera.AddPosition(0.f, +dx, 0.f);
	}
	if (Keyboard.IsKeyPressed(VK_SHIFT)) {
		Camera.AddPosition(0.f, -dx, 0.f);
	}

	if (Graphics.mRotationMode) Graphics.mSoldier.AddRotation(0.f, 0.0005f * dt, 0.f);

}

void CEngine::Render()
{
	Graphics.Render();
}
