#pragma once

#include <d3d11.h>

class MaterialDX11
{
public:
	MaterialDX11(){}
	~MaterialDX11(){}

	void Initialize(ID3D11Device* pDevice);

private:
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
};