#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include <vector>

struct MeshInfo;
class MeshDX11;
class Entity
{
public:
	Entity() : m_pMesh(nullptr){}
	~Entity(){}

	void SetMesh(MeshDX11* pMesh);
	void AttachChild(Entity* pChild);
	void Render(ID3D11DeviceContext* pContext);

private:
	void SetParent(Entity* pParent);

private:
	MeshDX11*				m_pMesh;
	XMMATRIX				m_Transform;

	Entity*					m_pParent;
	std::vector<Entity*>	m_Child;
};