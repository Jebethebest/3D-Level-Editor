#include "stdafx.h"
#include "QuadDrawer.h"
#include "Material.h"
#include "OverlordGame.h"


QuadDrawer::QuadDrawer()
{
}


QuadDrawer::~QuadDrawer()
{
	SafeRelease(m_pVertexBuffer);
}

void QuadDrawer::Initialize(ID3D11Device* pDevice)
{
	if (m_IsInitialized)
		return;

	m_ScreenWidth = static_cast<float>(OverlordGame::GetGameSettings().Window.Width);
	m_HalfScreenWidth = m_ScreenWidth * 0.5f;
	m_ScreenHeight = static_cast<float>(OverlordGame::GetGameSettings().Window.Height);
	m_HalfScreenHeight = m_ScreenHeight * 0.5f;

	//Vertices
	m_vertexArray[0] = VertexPosTex(DirectX::XMFLOAT3(-1, 1, 0), DirectX::XMFLOAT2(0, 0));
	m_vertexArray[1] = VertexPosTex(DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT2(1, 0));
	m_vertexArray[2] = VertexPosTex(DirectX::XMFLOAT3(-1, -1, 0), DirectX::XMFLOAT2(0, 1));
	m_vertexArray[3] = VertexPosTex(DirectX::XMFLOAT3(1, -1, 0), DirectX::XMFLOAT2(1, 1));

	//Vertex Buffer
	D3D11_BUFFER_DESC vertexBuffDesc;
	vertexBuffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.ByteWidth = sizeof(VertexPosTex) * 4;
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.MiscFlags = 0;

	auto hRes = pDevice->CreateBuffer(&vertexBuffDesc,nullptr, &m_pVertexBuffer);
	Logger::LogHResult(hRes, L"QuadDrawer::Initialize(...)");

	m_IsInitialized = true;
}

void QuadDrawer::DrawImmediate(ID3D11DeviceContext* pDeviceContext, Material* pMaterial)
{
	DrawImmediate(pDeviceContext, pMaterial, 0, 0, static_cast<int>(m_ScreenWidth), static_cast<int>(m_ScreenHeight));
}

void QuadDrawer::DrawImmediate(ID3D11DeviceContext* pDeviceContext, Material* pMaterial, int x, int y, int width, int height)
{
	//Update Vertices, based on screensize
	float ndcX = ((x - m_HalfScreenWidth) / m_ScreenWidth) * 2.0f;
	float ndcY = ((m_HalfScreenHeight - y) / m_ScreenHeight) *2.0f;
	float ndcWidth = (width / m_ScreenWidth) * 2.0f;
	float ndcHeight = (height / m_ScreenHeight) * 2.0f;

	m_vertexArray[0].Position.x = ndcX;
	m_vertexArray[0].Position.y = ndcY;
	m_vertexArray[1].Position.x = ndcX + ndcWidth;
	m_vertexArray[1].Position.y = ndcY;
	m_vertexArray[2].Position.x = ndcX;
	m_vertexArray[2].Position.y = ndcY - ndcHeight;
	m_vertexArray[3].Position.x = ndcX + ndcWidth;
	m_vertexArray[3].Position.y = ndcY - ndcHeight;

	//Update Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	memcpy(mappedResource.pData, &m_vertexArray[0], sizeof(VertexPosTex) * 4);
	pDeviceContext->Unmap(m_pVertexBuffer, 0);
 
	//Setup Pipeline
	//Set Inputlayout
	pDeviceContext->IASetInputLayout(pMaterial->GetInputLayout());

	//Set Vertex Buffer
	UINT offset = 0;
	UINT stride = sizeof(VertexPosTex);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set Primitive Topology
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//DRAW
	auto tech = pMaterial->GetTechnique();
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->Draw(4, 0);
	}
}
