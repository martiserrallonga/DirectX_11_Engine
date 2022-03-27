#include "Shaders.h"

bool CVertexShader::Init(Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath)
{
	HRESULT hr = D3DReadFileToBlob(aShaderPath.c_str(), mShaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		CErrorLogger::Log(hr, L"Failed to load shader: " + aShaderPath);
		return false;
	}

	hr = aDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), NULL, mShader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create vertex shader: ";
		errorMsg += aShaderPath;
		CErrorLogger::Log(hr, errorMsg);
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