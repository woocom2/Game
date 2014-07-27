#include "stdafx.h"

#include "MeshDX11.h"
#include "MeshInfo.h"
#include "ShaderDX11.h"

void MeshDX11::Initialize(ID3D11Device* pDevice, const MeshInfo& info)
{
	if ( info.IsValid())
	{
		D3D11_BUFFER_DESC  bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VertexData)* info.m_iVertCount;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = info.m_pVertices;

		if (FAILED(pDevice->CreateBuffer(&bufferDesc, &initData, &m_pVB)))
		{
			return;
		}

		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(UShort)* info.m_iTriCount * 3;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = info.m_pIndices;

		if (FAILED(pDevice->CreateBuffer(&bufferDesc, &initData, &m_pIB)))
		{
			return;
		}

		m_indexCount = info.m_iTriCount * 3;

		// 셰이더 세팅
		// 일단 하드코딩 나중에 모델 데이터에서 세팅된 메테리얼을 읽도록 수정
		m_pShader = new ShaderDX11();
		m_pShader->Initialize(pDevice, L"phongShader");
	}
}

void MeshDX11::Draw(ID3D11DeviceContext* pContext)
{
	UINT iOffset = 0;
	UINT iStride = sizeof(VertexData);

	pContext->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
	pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R16_UINT, 0);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pShader->SetShader(pContext);

	pContext->DrawIndexed(m_indexCount, 0, 0);

	//// Render the triangle.
	//deviceContext->DrawIndexed(indexCount, 0, 0);
}