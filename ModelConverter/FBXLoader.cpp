#include "stdafx.h"
#include "FbxLoader.h"
#include <assert.h>
#include <string>
#include <iostream>

#define FBXSDK_DESTROY( p ) { if( p ) { p->Destroy();} p = NULL; }

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
	std::cout << "Loader Initializing..." << std::endl;
	m_pMgr = FbxManager::Create();

	m_pIO = FbxIOSettings::Create(m_pMgr, IOSROOT);
	m_pMgr->SetIOSettings(m_pIO);

	m_pImporter = FbxImporter::Create(m_pMgr, "");
	std::cout << "Loader Initialized." << std::endl;
}

void FBXLoader::Destroy()
{
	FBXSDK_DESTROY(m_pScene);
	FBXSDK_DESTROY(m_pImporter);
	FBXSDK_DESTROY(m_pIO);
	FBXSDK_DESTROY(m_pMgr);
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
	std::cout << "Load Start. File :" << file << std::endl;
	bool bSuccess = m_pImporter->Initialize(file, -1, m_pIO);
	if (!bSuccess)
	{
		//char buffer[1024];
		//sprintf(buffer, "Error returned: %s", m_pImporter->GetLastErrorString());
		//MessageBoxA(NULL, buffer, "Call to FbxImporter::Initialize() failed.", MB_OK);
		std::cout << "Load Failed." << std::endl;

		return false;
	}
	
	std::cout << "Load end." << std::endl;

	if (m_pScene)
	{
		FBXSDK_DESTROY(m_pScene);
	}

	m_iVertexCnt = 0;
	m_iTriangleCnt = 0;

	std::string strFileName(file);
	size_t itr = strFileName.rfind('.');
	char buffer[256];
	snprintf(buffer, 256, "%s.vtx", strFileName.substr(0,itr).c_str());
	m_pFile = fopen(buffer, "w");

	std::cout << "Converting..." << std::endl;
	std::cout<<"OutputFile : " << buffer << std::endl;

	m_pScene = FbxScene::Create(m_pMgr, "LoadScene");
	m_pImporter->Import(m_pScene);

	std::cout << "Processing.";
	// Convert mesh, NURBS and patch into triangle mesh
	FbxGeometryConverter lGeomConverter(m_pMgr);
	lGeomConverter.Triangulate(m_pScene, /*replace*/true);

	// Split meshes per material, so that we only have one material per mesh (for VBO support)
	lGeomConverter.SplitMeshesPerMaterial(m_pScene, /*replace*/true);

	int iNodeCount = m_pScene->GetNodeCount();
	for (int i = 0; i < iNodeCount; ++i)
	{
		FbxNode* pNode = m_pScene->GetNode(i);
		RecursiveNode(pNode);
	}
	std::cout << std::endl;

	fclose(m_pFile);
	m_pFile = nullptr;

	std::cout << "Converting complete." << std::endl;
	//m_bFileLoaded = true;
	return true;
}

void FBXLoader::RecursiveNode(FbxNode* pNode)
{
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh)
	{
		std::cout << ".";
		Print("#NodeName %s\n", pNode->GetName());
		ReadMaterial(pNode);
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
	int iCount = pMesh->GetControlPointsCount();
	int iNormalCount = pMesh->GetElementNormalCount();
	int iUVCount = pMesh->GetElementUVCount();
	Print("#VCount %d\n", iCount);

	for (int i = 0; i < iCount; ++i)
	{
		FbxVector4 vec4 = pMesh->GetControlPointAt(i);
		Print("%f %f %f ", vec4.mData[0], vec4.mData[1], vec4.mData[2]);


		for (int j = 0; j < iNormalCount; ++j)
		{
			FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(j);
			if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				FbxVector4 nor = pNormal->GetDirectArray().GetAt(i);
				Print("%f %f %f ", nor.mData[0], nor.mData[1], nor.mData[2]);
			}
			else
			{
				int idx = pNormal->GetIndexArray().GetAt(i);
				FbxVector4 nor = pNormal->GetDirectArray().GetAt(idx);
				Print("%f %f %f ", nor.mData[0], nor.mData[1], nor.mData[2]);
			}
		}

		for (int j = 0; j < iUVCount; ++j)
		{
			FbxGeometryElementUV* pUV = pMesh->GetElementUV(j);
			if (pUV->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				FbxVector2 uv = pUV->GetDirectArray().GetAt(i);
				Print("%f %f\n", uv.mData[0], uv.mData[1]);
			}
			else
			{
				int idx = pUV->GetIndexArray().GetAt(i);
				FbxVector2 uv = pUV->GetDirectArray().GetAt(idx);
				Print("%f %f\n", uv.mData[0], uv.mData[1]);
			}
		}
	}

	// ÀÎµ¦½º
	int iVertCnt = pMesh->GetPolygonVertexCount();
	int* pIndices = pMesh->GetPolygonVertices();

	int iPolyCnt = pMesh->GetPolygonCount();
	Print("#TriCount %d\n", iPolyCnt);
	for (int vertCount = 0; vertCount < iVertCnt; vertCount += 3)
	{
		Print("%d/%d/%d ", pIndices[vertCount], pIndices[vertCount + 1], pIndices[vertCount + 2]);
	}
	Print("\n#End\n");
}

void FBXLoader::ReadMaterial(FbxNode* pNode)
{
	int iMaterialCount = pNode->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < iMaterialCount; ++i)
	{	
		FbxSurfaceMaterial* pMaterial = pNode->GetSrcObject<FbxSurfaceMaterial>(i);
		if (pMaterial)
		{
			FbxProperty property = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			int iLayeredTexture = property.GetSrcObjectCount<FbxLayeredTexture>();
			if (iLayeredTexture > 0)
			{
				for (int j = 0; j < iLayeredTexture; j++)
				{
					FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(j);
					int lcount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();
					for (int k = 0; k < lcount; k++)
					{
						FbxFileTexture* texture = layeredTexture->GetSrcObject<FbxFileTexture>(k);
						// Then, you can get all the properties of the texture, include its name

						std::string textureName(texture->GetFileName());
						size_t ipos = textureName.rfind('\\');
						Print("#Texture %s\n", textureName.substr(ipos+1).c_str());
					}
				}
			}
			else
			{
				// Directly get textures
				int textureCount = property.GetSrcObjectCount<FbxFileTexture>();
				for (int j = 0; j < textureCount; j++)
				{
					const FbxFileTexture* texture = property.GetSrcObject<FbxFileTexture>(j);
					// Then, you can get all the properties of the texture, include its name
					
					std::string textureName(texture->GetFileName());
					size_t ipos = textureName.rfind('\\');
					Print("#Texture %s\n", textureName.substr(ipos+1).c_str());
				}
			}
		}
	}
}