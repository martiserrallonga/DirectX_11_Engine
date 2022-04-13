#include "CModel.h"

bool CModel::Init(
	ID3D11Device* aDevice,
	ID3D11DeviceContext* aDeviceContext,
	ID3D11ShaderResourceView* aTexture,
	CConstantBuffer<CBVertexShader>& aCBVertexShader
)
{
	mDevice = aDevice;
	mDeviceContext = aDeviceContext;
	mTexture = aTexture;
	mCBVertexShader = &aCBVertexShader;

	try
	{
		TVertex Vertex[]
		{
			TVertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
			TVertex(-0.5f, +0.5f, -0.5f, 0.0f, 0.0f),
			TVertex(+0.5f, +0.5f, -0.5f, 1.0f, 0.0f),
			TVertex(+0.5f, -0.5f, -0.5f, 1.0f, 1.0f),

			TVertex(-0.5f, -0.5f, +0.5f, 0.0f, 1.0f),
			TVertex(-0.5f, +0.5f, +0.5f, 0.0f, 0.0f),
			TVertex(+0.5f, +0.5f, +0.5f, 1.0f, 0.0f),
			TVertex(+0.5f, -0.5f, +0.5f, 1.0f, 1.0f),
		};

		HRESULT hr = mVertexBuffer.Init(aDevice, Vertex, ARRAYSIZE(Vertex));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

		DWORD Index[]
		{
			0, 1, 2,
			0, 2, 3,
			1, 5, 6,
			1, 6, 2,
			2, 6, 7,
			2, 7, 3,
			4, 0, 3,
			4, 3, 7,
			1, 0, 4,
			1, 4, 5,
			4, 7, 6,
			4, 6, 5,
		};

		hr = mIndexBuffer.Init(aDevice, Index, ARRAYSIZE(Index));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");
	}
	catch (CComException& exception)
	{
		CErrorLogger::Log(exception);
		return false;
	}

	UpdateWorldMatrix();
	return true;
}

void CModel::SetTexture(ID3D11ShaderResourceView* aTexture)
{
	mTexture = aTexture;
}

void CModel::Render(const XMMATRIX& aViewProjectionMatrix)
{
	mCBVertexShader->mData.Transform = XMMatrixTranspose(mWorldMatrix * aViewProjectionMatrix);
	mCBVertexShader->Update();
	
	UINT Offset = 0;
	mDeviceContext->VSSetConstantBuffers(0, 1, mCBVertexShader->GetAddressOf());
	mDeviceContext->PSSetShaderResources(0, 1, &mTexture);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &Offset);
	mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);
}

void CModel::UpdateWorldMatrix()
{
	mWorldMatrix = XMMatrixIdentity();
}