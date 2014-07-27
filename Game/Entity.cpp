#include "stdafx.h"
#include "Entity.h"
#include "MeshDX11.h"

void Entity::SetMesh(MeshDX11* pMesh)
{
	if (pMesh)
	{
		m_pMesh = pMesh;
	}
}

void Entity::SetParent(Entity* pParent)
{
	if (pParent)
	{
		m_pParent = pParent;
	}
}

void Entity::AttachChild(Entity* pChild)
{
	if (pChild)
	{
		pChild->SetParent(this);
		m_Child.push_back(pChild);
	}
}

void Entity::Render(ID3D11DeviceContext* pContext)
{
	if (m_pMesh)
	{
		m_pMesh->Draw(pContext);
	}

	size_t iCount = m_Child.size();
	for (UINT i = 0; i < iCount; ++i)
	{
		if (m_Child[i])
		{
			m_Child[i]->Render(pContext);
		}
	}
}

bool Entity::IsEmpty()
{
	if (m_pMesh)
	{
		// mesh가 존재하면 비어있지 않음
		return false;
	}
	else
	{
		size_t iCount = m_Child.size();
		for (UINT i = 0; i < iCount; ++i)
		{
			if (m_Child[i])
			{
				return m_Child[i]->IsEmpty();
			}
		}
	}

	return true;
}