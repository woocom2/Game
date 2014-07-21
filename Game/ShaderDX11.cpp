#include "stdafx.h"
#include "ShaderDX11.h"

#include <d3dx11.h>
#include <d3dcompiler.h>

void ShaderDX11::Initialize(ID3D11Device* pDevice, const wchar_t* name)
{
	ID3DBlob* pVertexShaderBuffer = nullptr;
	CompileShader(L"phongShader.vs", &pVertexShaderBuffer);
	pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), 
		pVertexShaderBuffer->GetBufferSize(), nullptr, &m_pVertexShader);
	
	ID3DBlob* pPixelShaderBuffer = nullptr;
	CompileShader(L"phongShader.ps", &pPixelShaderBuffer);
	pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), 
		pPixelShaderBuffer->GetBufferSize(), nullptr, &m_pPixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	pDevice->CreateInputLayout(layout, numElements, 
		pVertexShaderBuffer->GetBufferPointer(), 
		pVertexShaderBuffer->GetBufferSize(), &m_pInputLayout);

	pVertexShaderBuffer->Release();
	pPixelShaderBuffer->Release();
}

HRESULT ShaderDX11::CompileShader(const wchar_t* name, ID3DBlob** ppBlob)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;

	size_t nameLen = wcslen(name);
	const wchar_t* shaderType = &name[nameLen - 2];

	if (wcsncmp(shaderType, L"vs", 2) == 0)
	{
		hr = D3DX11CompileFromFile(name, NULL, NULL, "VS_Main", "vs_5_0",
			dwShaderFlags, 0, NULL, ppBlob, &pErrorBlob, NULL);
	}
	else if (wcsncmp(shaderType, L"ps", 2) == 0)
	{
		hr = D3DX11CompileFromFile(name, NULL, NULL, "PS_Main", "vs_5_0",
			dwShaderFlags, 0, NULL, ppBlob, &pErrorBlob, NULL);
	}
	else
	{
		return S_FALSE;
	}
	
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void ShaderDX11::SetShader(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pInputLayout);

	pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);
}