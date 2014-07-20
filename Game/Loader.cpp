#include "stdafx.h"

#include "Loader.h"
#include "MeshInfo.h"
#include "MeshDX11.h"
#include "Entity.h"

#include <fstream>

Entity* Loader::LoadModel(const wchar_t* filename) const
{
	std::wfstream file(filename);
	
	const int bufferSize = 128;
	wchar_t buffer[bufferSize] = { 0, };

	// create Entity
	Entity* pRoot = new Entity();
	
	while (file.good())
	{
		// read word
		file >> buffer;

		if (wcsncmp(buffer, L"#NodeName", 9) == 0)
		{
			file >> buffer;
			MeshInfo meshInfo(buffer);
			
			// read file - body loop
			while (file.good())
			{
				file >> buffer;
				if (wcsncmp(buffer, L"#Texture", 8) == 0)
				{
					file >> buffer;
					meshInfo.m_TextureName = buffer;
				}
				else if (wcsncmp(buffer, L"#VCount", 7) == 0)
				{
					// 정점 갯수만큼 Data 배열 생성
					unsigned int iVertexCount = 0;
					file >> iVertexCount;
					VertexData* pVData = new VertexData[iVertexCount];
					
					for (unsigned int iCount = 0; iCount < iVertexCount; ++iCount)
					{
						file >> pVData[iCount].m_Position.x >> pVData[iCount].m_Position.y >> pVData[iCount].m_Position.z
							>> pVData[iCount].m_Normal.x >> pVData[iCount].m_Normal.y >> pVData[iCount].m_Normal.z
							>> pVData[iCount].m_TexCoord.x >> pVData[iCount].m_TexCoord.y;
					}

					// vertex data assign
					meshInfo.m_pVertices = pVData;
					meshInfo.m_iVertCount = iVertexCount;
				}
				else if (wcsncmp(buffer, L"#TriCount", 9) == 0)
				{
					unsigned int iTriCount = 0;
					file >> iTriCount;

					UShort* pIdx = new UShort[iTriCount * 3];

					for (unsigned int iCount = 0; iCount < iTriCount; ++iCount)
					{
						file >> buffer;
						wscanf_s(buffer, "%d/%d/%d", &pIdx[iCount], &pIdx[iCount+1], &pIdx[iCount+2]);
					}

					// index data assign
					meshInfo.m_pIndices = pIdx;
					meshInfo.m_iTriCount = iTriCount;
				}
				else if (wcsncmp(buffer, L"#End", 4) == 0)
				{
					break;
				}
			}

			// DX11
			MeshDX11* pMesh = new MeshDX11();
			pMesh->Initialize(m_pDevice, meshInfo);

			Entity* pChild = new Entity();
			pChild->SetMesh(pMesh);
			pRoot->AttachChild(pChild);
		}

	}
	return nullptr;
}