// ModelConverter.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "FbxLoader.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 1)
	{
		FBXLoader loader;
		loader.Initialize();
		loader.LoadFile(argv[1]);
		loader.Destroy();
	}
	system("pause");

	return 0;
}

