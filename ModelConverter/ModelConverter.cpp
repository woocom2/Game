// ModelConverter.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

