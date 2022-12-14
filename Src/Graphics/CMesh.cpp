#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CComException.h"

CMesh::CMesh(ID3D11Device* aDevice,
	ID3D11DeviceContext* aDeviceContext,
	std::vector<TVertex> aVertices,
	std::vector<DWORD>& aIndices,
	std::vector<CTexture>& aTextures,
	const DirectX::XMMATRIX& aTransform)
	: CTransform(aTransform)
	, mDeviceContext(aDeviceContext)
	, mTextures(aTextures)
{
	HRESULT hr;
	hr = mVertexBuffer.Init(aDevice, aVertices.data(), aVertices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");
	hr = mIndexBuffer.Init(aDevice, aIndices.data(), aIndices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
}

void CMesh::Render() const
{
	for (auto& texture : mTextures) {
		if (texture.GetType() == aiTextureType::aiTextureType_DIFFUSE) {
			mDeviceContext->PSSetShaderResources(0, 1, texture.GetTextureViewAdress());
			break;
		}
	}

	UINT Offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &Offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	mDeviceContext->DrawIndexed(mIndexBuffer.GetIndexCount(), 0, 0);
}
