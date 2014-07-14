#include "stdafx.h"

#include "Loader.h"
#include "MeshInfo.h"

#include <fstream>
#include <vector>

void ReadVertex(const wchar_t* buffer, std::vector<Float3>& vertices)
{
	float pos[3] = { 0, };
	wscanf_s(buffer, "v %f %f %f", &pos[0], &pos[1], &pos[2]);
	vertices.emplace_back(pos[0], pos[1], pos[2]);
}

void ReadNormal(const wchar_t* buffer, std::vector<Float3>& normals)
{
	float nor[3] = { 0, };
	wscanf_s(buffer, "vn %f %f %f", &nor[0], &nor[1], &nor[2]);
	normals.emplace_back(nor[0], nor[1], nor[2]);
}

const MeshInfo* Loader::LoadObj(const wchar_t* filename) const
{
	std::wfstream file(filename);
	
	const int bufferSize = 256;
	wchar_t buffer[bufferSize] = { 0, };
	
	std::vector<Float3> vertices;
	std::vector<Float3> normals;
	std::vector<Float2> texCoords;

	while (file.good())
	{
		file.getline(buffer, bufferSize);

		if (wcsncmp(buffer, L"v", 1) == 0)
		{
			ReadVertex(buffer, vertices);
		}
		else if (wcsncmp(buffer, L"vn", 2) == 0)
		{
			ReadNormal(buffer, normals);
		}
	}
	return nullptr;
}