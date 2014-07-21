#pragma once

#include <d3d11.h>

class ShaderDX11
{
public:
	ShaderDX11(){}
	~ShaderDX11(){}

	void	Initialize(ID3D11Device* pDevice, const wchar_t* name);

private:
	HRESULT CompileShader(const wchar_t* name, ID3DBlob** ppBlob);

private:
	ShaderDX11(const ShaderDX11& other);
	ShaderDX11& operator=(const ShaderDX11& other);

private:
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
};