#pragma once

#include "IMesh.h"
#include <d3d11.h>

class MaterialDX11;
class MeshDX11 : public IMesh
{
public:
	MeshDX11(){}
	~MeshDX11(){}

	virtual void Initialize(const MeshInfo& info);
	virtual void Draw();

private:
	// �������, ��������
	MeshDX11(const MeshDX11& other);
	MeshDX11& operator =(const MeshDX11& other);

private:
	ID3D11Buffer*		m_pVB;
	ID3D11Buffer*		m_pIB;
	MaterialDX11*		m_pMaterial;
};