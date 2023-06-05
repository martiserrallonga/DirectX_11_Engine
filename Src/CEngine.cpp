#include "CEngine.h"
#include "Utils/Format.h"

bool CEngine::Init(std::string configPath, HINSTANCE hInstance)
{
	_configPath = std::move(configPath);
	json config = Format::JsonFile(_configPath);

	if (!Window.Init(config.at("WindowConfig"), hInstance, Input)) return false;
	if (!Graphics.Init(config.at("GraphicsConfig"), Window)) return false;

	Timer.Start();
	return true;
}

bool CEngine::ProcessMessages()
{
	return Window.ProcessMessages();
}

void CEngine::Update()
{
	auto dt = static_cast<float>(Timer.GetMiliseconds());
	Timer.Restart();

	// Input Move Camera 
	CCamera& Camera = Graphics.Camera;
	const float dx = Camera.Speed / 1000.f * dt;

	while (!Input.Mouse.IsEventBufferEmpty()) {
		CMouseEvent Event = Input.Mouse.ReadEvent();
		if (Input.Mouse.IsRightPressed()
			&& Event.GetType() == CMouseEvent::EEventType::RawMove)
		{
			auto pos = Event.GetPos();
			float pitch = static_cast<float>(pos.y) * DirectX::XM_PI / 180.f;
			float yaw = static_cast<float>(pos.x) * DirectX::XM_PI / 180.f;
			Camera.AddRotation(pitch, yaw, 0.f);
		}
	}

	if (Input.Keyboard.IsKeyPressed('W')) {
		Camera.AddPosition(+Camera.GetForward() * dx);
	}
	if (Input.Keyboard.IsKeyPressed('S')) {
		Camera.AddPosition(-Camera.GetForward() * dx);
	}
	if (Input.Keyboard.IsKeyPressed('A')) {
		Camera.AddPosition(-Camera.GetRight() * dx);
	}
	if (Input.Keyboard.IsKeyPressed('D')) {
		Camera.AddPosition(+Camera.GetRight() * dx);
	}
	if (Input.Keyboard.IsKeyPressed(VK_SPACE)) {
		Camera.AddPosition(0.f, +dx, 0.f);
	}
	if (Input.Keyboard.IsKeyPressed(VK_SHIFT)) {
		Camera.AddPosition(0.f, -dx, 0.f);
	}

	if (Input.Keyboard.IsKeyPressed('L')) {
		auto& light = Graphics.mBulb;
		light.SetPosition(Camera.GetPositionVector() + Camera.GetForward());
		light.SetRotation(Camera.GetRotationVector());
	}

	if (Graphics.mRotationMode) Graphics.mSoldier.AddRotation(0.f, 0.0005f * dt, 0.f);

}

void CEngine::Render()
{
	Graphics.Render();
}
