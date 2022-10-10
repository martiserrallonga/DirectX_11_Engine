#include "CGameEntity.h"

bool CGameEntity::Init(
	const std::string& aFilePath
	, ID3D11Device* aDevice
	, ID3D11DeviceContext* aDeviceContext
	, ID3D11ShaderResourceView* aTexture
	, CConstantBuffer<CBVertexShader>& aCBVertexShader
){
	if (!mModel.Init(aFilePath, aDevice, aDeviceContext, aTexture, aCBVertexShader)) return false;

	SetPosition(ZERO);
	SetRotation(ZERO);
	return true;
}

void CGameEntity::Render(const XMMATRIX& aViewProjectionMatrix) const
{
	mModel.Render(GetTransformMatrix() * aViewProjectionMatrix);
}
