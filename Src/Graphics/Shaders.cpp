#include "Shaders.h"

bool CVertexShader::Init(
	Microsoft::WRL::ComPtr<ID3D11Device>& aDevice,
	std::wstring aShaderPath,
	D3D11_INPUT_ELEMENT_DESC* aLayoutDesc,
	UINT aNumElements
)
{
	HRESULT hr = D3DReadFileToBlob(aShaderPath.c_str(), mShaderBuffer.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, L"Failed to load shader: " + aShaderPath);
		return false;
	}

	hr = aDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), NULL, mShader.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, L"Failed to create vertex shader: " + aShaderPath);
		return false;
	}

	hr = aDevice->CreateInputLayout(
		aLayoutDesc,
		aNumElements,
		mShaderBuffer->GetBufferPointer(),
		mShaderBuffer->GetBufferSize(),
		mInputLayout.GetAddressOf()
	);

	if (FAILED(hr)) {
		CErrorLogger::Log(hr, "Failed to create Input Layout");
		return false;
	}

	return true;
}

ID3D11VertexShader* CVertexShader::GetShader()
{
	return mShader.Get();
}

ID3D10Blob* CVertexShader::GetBuffer()
{
	return mShaderBuffer.Get();
}

ID3D11InputLayout* CVertexShader::GetInputLayout()
{
	return mInputLayout.Get();
}



bool CPixelShader::Init(Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath)
{
	HRESULT hr = D3DReadFileToBlob(aShaderPath.c_str(), mShaderBuffer.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, L"Failed to load shader: " + aShaderPath);
		return false;
	}

	hr = aDevice->CreatePixelShader(mShaderBuffer.Get()->GetBufferPointer(), mShaderBuffer.Get()->GetBufferSize(), NULL, mShader.GetAddressOf());
	if (FAILED(hr)) {
		CErrorLogger::Log(hr, L"Failed to create pixel shader: " + aShaderPath);
		return false;
	}

	return true;
}

ID3D11PixelShader* CPixelShader::GetShader()
{
	return mShader.Get();
}

ID3D10Blob* CPixelShader::GetBuffer()
{
	return mShaderBuffer.Get();
}
