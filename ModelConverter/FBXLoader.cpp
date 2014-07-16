#include "stdafx.h"
#include "FbxLoader.h"
#include <assert.h>
#include <string>

#define FBXSDK_DESTROY( p ) { if( p ) { p->Destroy();} p = nullptr; }

char* WCharToChar(const wchar_t* pwstrSrc)
{
	assert(pwstrSrc);

#if !defined _DEBUG
	int len = 0;
	len = (wcslen(pwstrSrc) + 1) * 2;
	char* pstr = (char*)malloc(sizeof(char)* len);

	WideCharToMultiByte(949, 0, pwstrSrc, -1, pstr, len, NULL, NULL);
#else

	int nLen = wcslen(pwstrSrc);

	char* pstr = (char*)malloc(sizeof(char)* nLen + 1);
	wcstombs(pstr, pwstrSrc, nLen + 1);
#endif

	return pstr;
}

void FBXLoader::Print(const char* text, ...)
{
	va_list arg;

	va_start(arg, text);
	const int bufferSize = 2048;
	char buffer[bufferSize];
	size_t iPos = 0;
	for (const char* p = text; *p; ++p)
	{
		if (*p == '%')
		{
			++p;
			switch (*p)
			{
			case 'd':
				_snprintf(buffer + iPos, bufferSize, "%d", va_arg(arg, long));
				iPos = strlen(buffer);
				break;
			case 'f':
				_snprintf(buffer + iPos, bufferSize, "%f", va_arg(arg, double));
				iPos = strlen(buffer);
				break;
			case 's':
				_snprintf(buffer + iPos, bufferSize, "%s", va_arg(arg, char*));
				iPos = strlen(buffer);
			}
		}
		else
		{
			buffer[iPos++] = *p;
		}
	}

	buffer[iPos] = NULL;
	//printf( buffer);
	fprintf_s(m_pFile, buffer);

	va_end(arg);
}

void FBXLoader::Initialize()
{
	m_pMgr = FbxManager::Create();

	m_pIO = FbxIOSettings::Create(m_pMgr, IOSROOT);
	m_pMgr->SetIOSettings(m_pIO);

	m_pImporter = FbxImporter::Create(m_pMgr, "");
}

void FBXLoader::Destroy()
{
	FBXSDK_DESTROY(m_pImporter);
	FBXSDK_DESTROY(m_pMgr);
	FBXSDK_DESTROY(m_pIO);
	FBXSDK_DESTROY(m_pMgr);
}

bool FBXLoader::LoadFile(const wchar_t* file)
{
	const char* szFile = WCharToChar(file);
	bool bSucceed = LoadFile(szFile);
	
	delete[] szFile;
	
	return bSucceed;
}

bool FBXLoader::LoadFile(const char* file)
{
	bool bSuccess = m_pImporter->Initialize(file, -1, m_pIO);
	if (!bSuccess)
	{
		//char buffer[1024];
		//sprintf(buffer, "Error returned: %s", m_pImporter->GetLastErrorString());
		//MessageBoxA(NULL, buffer, "Call to FbxImporter::Initialize() failed.", MB_OK);

		return false;
	}

	if (m_pScene)
		m_pScene->Destroy();

	m_iVertexCnt = 0;
	m_iTriangleCnt = 0;

	std::string strFileName(file);
	size_t itr = strFileName.find('.');
	char buffer[256];
	snprintf(buffer, 256, "%s.vtx", strFileName.substr(0,itr).c_str());
	m_pFile = fopen(buffer, "w");

	m_pScene = FbxScene::Create(m_pMgr, "LoadScene");
	m_pImporter->Import(m_pScene);

	// Convert mesh, NURBS and patch into triangle mesh
	FbxGeometryConverter lGeomConverter(m_pMgr);
	lGeomConverter.Triangulate(m_pScene, /*replace*/true);

	// Split meshes per material, so that we only have one material per mesh (for VBO support)
	//lGeomConverter.SplitMeshesPerMaterial(m_pScene, /*replace*/true);

	int iNodeCount = m_pScene->GetNodeCount();
	for (int i = 0; i < iNodeCount; ++i)
	{
		FbxNode* pNode = m_pScene->GetNode(i);
		RecursiveNode(pNode);
	}

	fclose(m_pFile);
	m_pFile = nullptr;

	//m_bFileLoaded = true;
	return true;
}

void FBXLoader::RecursiveNode(FbxNode* pNode)
{
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh)
	{
		ReadMesh(pMesh);
	}

	int iChildCount = pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		RecursiveNode(pNode->GetChild(i));
	}
}

void FBXLoader::ReadMesh(FbxMesh* pMesh)
{
	Print("Mesh Name : %s\n", pMesh->GetName());
	int iCount = pMesh->GetControlPointsCount();
	Print("Vertex count : %d\n", iCount);
	
	for (int i = 0; i < iCount; ++i)
	{
		FbxVector4 vec4 = pMesh->GetControlPointAt(i);
		Print("%d : ", i);
		Print("%f, %f, %f ", vec4.mData[0], vec4.mData[1], vec4.mData[2]);

		int iNormalCount = pMesh->GetElementNormalCount();
		for (int j = 0; j < iNormalCount; ++j)
		{
			FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(j);
			if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				FbxVector4 nor = pNormal->GetDirectArray().GetAt(i);
				Print("%f, %f, %f ", nor.mData[0], nor.mData[1], nor.mData[2]);
			}
			else
			{
				int idx = pNormal->GetIndexArray().GetAt(i);
				FbxVector4 nor = pNormal->GetDirectArray().GetAt(idx);
				Print("%f, %f, %f ", nor.mData[0], nor.mData[1], nor.mData[2]);
			}
		}

		int iUVCount = pMesh->GetElementUVCount();
		for (int j = 0; j < iUVCount; ++j)
		{
			FbxGeometryElementUV* pUV = pMesh->GetElementUV(j);
			if (pUV->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				FbxVector2 uv = pUV->GetDirectArray().GetAt(i);
				Print("%f, %f\n", uv.mData[0], uv.mData[1]);
			}
			else
			{
				int idx = pUV->GetIndexArray().GetAt(i);
				FbxVector2 uv = pUV->GetDirectArray().GetAt(idx);
				Print("%f, %f\n", uv.mData[0], uv.mData[1]);
			}
		}
	}
}