#include "CGameEntity.h"

bool CGameEntity::Init(
	const std::string& aFilePath
	, ID3D11Device* aDevice
	, ID3D11DeviceContext* aDeviceContext
	, CConstantBuffer<CBVertexShader>& aCBVertexShader
){
	if (!mModel.Init(aFilePath, aDevice, aDeviceContext, aCBVertexShader)) return false;

	SetPosition(300.f, 200.f, 100.f);
	SetRotation(0.2f, 0.5f, 0.f);
	return true;
}

void CGameEntity::Render(const XMMATRIX& aViewProjectionMatrix) const {
	mModel.Render(GetTransformMatrix() * aViewProjectionMatrix);
}
