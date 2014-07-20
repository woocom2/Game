#pragma once

#include <string>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p){ if(p){ delete p; p = NULL;}}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p){ if(p){ delete[] p; p=NULL;}}
#endif

struct Float3
{
	Float3(float _x, float _y, float _z)
	:x(_x), y(_y), z(_z){}
	Float3() :x(0.0f), y(0.0f), z(0.0f){}

	float x, y, z;
};

struct Float2
{
	Float2(float _x, float _y)
	:x(_x), y(_y){}
	Float2() :x(0.0f), y(0.0f){}

	float x, y;
};

struct VertexData
{
	Float3 m_Position;
	Float3 m_Normal;
	Float2 m_TexCoord;
};

typedef unsigned short UShort;
struct MeshInfo
{
	MeshInfo(const wchar_t* name) 
	: m_Name(name),m_iTriCount(0), m_iVertCount(0),
	m_pVertices(nullptr),m_pIndices(nullptr){}
	
	~MeshInfo()
	{
		SAFE_DELETE_ARRAY(m_pVertices);
		SAFE_DELETE_ARRAY(m_pIndices);
	}

	bool IsValid() const
	{
		return m_iVertCount > 0 && m_iTriCount > 0;
	}

	std::wstring	m_Name;
	std::wstring	m_TextureName;

	VertexData*		m_pVertices;
	UShort*			m_pIndices;

	unsigned int	m_iVertCount;
	unsigned int	m_iTriCount;
};