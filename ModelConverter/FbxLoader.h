#pragma once

#include <fbxsdk.h>
#include <fbxsdk\fileio\fbxiosettings.h>

class FBXLoader
{
public:
	FBXLoader(){}
	~FBXLoader(){}

	void Initialize();
	void Destroy();

	bool LoadFile(const char* file);
	bool LoadFile(const wchar_t* file);
	void RecursiveNode(FbxNode* pNode);
	void ReadMesh(FbxMesh* pMesh);
	void Print(const char* text, ...);

private:
	FbxManager*		m_pMgr;
	FbxIOSettings*	m_pIO;
	FbxImporter*	m_pImporter;
	FbxScene*		m_pScene;

	FILE*			m_pFile;

	unsigned long	m_iVertexCnt;
	unsigned long	m_iTriangleCnt;
};