#pragma once

#include "CModel.h"

class CGameEntity : public CTransform
{
public:
	bool Init(
		const std::string &aFilePath
		, ID3D11Device* aDevice
		, ID3D11DeviceContext* aDeviceContext
		, ID3D11ShaderResourceView* aTexture
		, CConstantBuffer<CBVertexShader>& aCBVertexShader
	);

	void Render(const XMMATRIX& aViewProjectionMatrix) const;
	
protected:
	void UpdateMatrix() override { UpdateWorldMatrix(); }

private:
	CModel mModel;

};