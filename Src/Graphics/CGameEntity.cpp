#include "CGameEntity.h"

bool CGameEntity::Init(
	const std::string& aFilePath
	, ID3D11Device* aDevice
	, ID3D11DeviceContext* aDeviceContext
	, CConstantBuffer<CBVertexShader>& aCBVertexShader
){
	if (!mModel.Init(aFilePath, aDevice, aDeviceContext, aCBVertexShader)) return false;

	return true;
}

void CGameEntity::Render(const XMMATRIX& aViewProjectionMatrix) const {
	mModel.Render(GetTransformMatrix(), aViewProjectionMatrix);
}
