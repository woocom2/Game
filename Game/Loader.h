#pragma once

#include <d3d11.h>
#include "common.h"

class Entity;
class Loader
{
public:
	Loader(ID3D11Device* pDevice) : m_pDevice(pDevice){}
	~Loader(){}

	Entity* LoadModel(const wchar_t* filename) const;

private:
	ID3D11Device*		m_pDevice;
};