#pragma once
#include "CErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")


class CVertexShader
{
public:
	bool Init(
		Microsoft::WRL::ComPtr<ID3D11Device>& aDevice,
		std::wstring aShaderPath,
		D3D11_INPUT_ELEMENT_DESC* aLayoutDesc,
		UINT aNumElements
	);
	
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> mShaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

};

class CPixelShader
{
public:
	bool Init(Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> mShaderBuffer;
};