#pragma once

#include <d3d11.h>

struct MeshInfo;
class ShaderDX11;
class MeshDX11
{
public:
	MeshDX11() : m_pVB(nullptr), m_pIB(nullptr), m_pShader(nullptr){}
	~MeshDX11(){}

	void Initialize(ID3D11Device* pDevice, const MeshInfo& info);
	void Draw(ID3D11DeviceContext* pContext);

private:
	// �������, ��������
	MeshDX11(const MeshDX11& other);
	MeshDX11& operator =(const MeshDX11& other);

private:
	UINT				m_indexCount;
	ID3D11Buffer*		m_pVB;
	ID3D11Buffer*		m_pIB;
	ShaderDX11*			m_pShader;
};