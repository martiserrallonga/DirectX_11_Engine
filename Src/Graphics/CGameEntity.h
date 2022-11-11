#pragma once

#include "CModel.h"

class CGameEntity : public CTransform
{
public:
	bool Init(
		const std::string &aFilePath
		, ID3D11Device* aDevice
		, ID3D11DeviceContext* aDeviceContext
		, CConstantBuffer<CBVertexShader>& aCBVertexShader
	);

	void Render(const XMMATRIX& aViewProjectionMatrix) const;
	
private:
	CModel mModel;

};