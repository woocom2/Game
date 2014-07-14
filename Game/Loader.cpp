#include "stdafx.h"

#include "Loader.h"

const MeshInfo* Loader::LoadObj(const wchar_t* filename) const
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, filename, L"r");
	
	if (pFile)
	{

	}

	return nullptr;
}