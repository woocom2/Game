#pragma once

#include "IRenderer.h"
#include <d3d11.h>

class RendererDX11 : public IRenderer
{
public:
	RendererDX11() : m_hWnd(nullptr), m_pDevice(nullptr), m_pContext(nullptr) {}
	virtual ~RendererDX11(){}

	HRESULT				Initialize(HWND hWnd);
	void				Render();
	void				Destroy();
	ID3D11Device*		GetDevice() const { return m_pDevice; }

private:
	HWND					m_hWnd;
	D3D_DRIVER_TYPE			m_DriverType;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;
	ID3D11Texture2D*		m_pDepthBuffer;
	ID3D11DepthStencilView*	m_pDepthStencilView;
	D3D11_VIEWPORT			m_ViewPort;
};