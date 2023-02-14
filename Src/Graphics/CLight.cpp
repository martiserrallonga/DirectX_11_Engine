#include "CLight.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


void CLight::Init(CTransform* aTransform, CConstantBuffer<CBLight>* aConstBuffer) {
	mpTransform = aTransform;
	mpConstBuffer = aConstBuffer;

	mAmbientColor = { 1.f, 1.f, 1.f };
	mAmbientStrength = 0.1f;

	mDirectionalColor = { 1.f, 1.f, 1.f };
	mDirectionalStrength = 1.f;

	mAttenuation_A = 0.2f;
	mAttenuation_B = 0.05f;
	mAttenuation_C = 0.3f;
}

void CLight::Update() const {
	auto& data = mpConstBuffer->mData;
	data.AmbientColor = mAmbientColor;
	data.AmbientStrength = mAmbientStrength;
	data.DirectionalColor = mDirectionalColor;
	data.DirectionalStrength = mDirectionalStrength;
	data.DirectionalPosition = mpTransform->GetPositionFloat3();
	data.Attenuation_A = mAttenuation_A;
	data.Attenuation_B = mAttenuation_B;
	data.Attenuation_C = mAttenuation_C;
}

void CLight::RenderImGui()
{
	ImGui::Text("Light Control");
	if (
		ImGui::ColorEdit3("Ambient Color", &mAmbientColor.x) ||
		ImGui::DragFloat("Ambient Strenght", &mAmbientStrength, 0.01f, 0.f, 1.f) ||
		ImGui::ColorEdit3("Directional Color", &mDirectionalColor.x) ||
		ImGui::DragFloat("Directional Strenght", &mDirectionalStrength, 0.01f, 0.f, 1.f) ||
		ImGui::DragFloat3("Directional Position", &mDirectionalPosition.x, 0.1f) ||
		ImGui::DragFloat("Attenuation_A", &mAttenuation_A, 0.01f, 0.f, 10000.f) ||
		ImGui::DragFloat("Attenuation_B", &mAttenuation_B, 0.01f, 0.f, 10000.f) ||
		ImGui::DragFloat("Attenuation_C", &mAttenuation_C, 0.01f, 0.01f, 10000.f)
		) {
		Update();
	}
}
