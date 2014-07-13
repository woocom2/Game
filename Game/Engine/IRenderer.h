#pragma once

#include <windef.h>

class IRenderer
{
public:
	IRenderer(){}
	~IRenderer(){}

	virtual HRESULT Initialize(HWND hWnd) = 0;
	virtual void	Render() = 0;
	virtual void	Destroy() = 0;
};