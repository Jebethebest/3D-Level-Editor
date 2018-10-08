#include "stdafx.h"
#include "PostProcessingMaterial.h"
#include "RenderTarget.h"
#include "OverlordGame.h"
#include "ContentManager.h"

PostProcessingMaterial::PostProcessingMaterial(std::wstring effectFile, unsigned int renderIndex,
                                               std::wstring technique)
	: m_IsInitialized(false), 
	  m_pInputLayout(nullptr),
	  m_pInputLayoutSize(0),
	  m_effectFile(std::move(effectFile)),
	  m_InputLayoutID(0),
	  m_RenderIndex(renderIndex),
	  m_pRenderTarget(nullptr),
	  m_pVertexBuffer(nullptr),
	  m_pIndexBuffer(nullptr),
	  m_NumVertices(0),
	  m_NumIndices(0),
	  m_VertexBufferStride(0),
	  m_pEffect(nullptr),
	  m_pTechnique(nullptr),
	  m_TechniqueName(std::move(technique))
{
}

PostProcessingMaterial::~PostProcessingMaterial()
{
	SafeRelease(m_pInputLayout);
	m_pInputLayoutDescriptions.clear();
	SafeDelete(m_pRenderTarget);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void PostProcessingMaterial::Initialize(const GameContext& gameContext)
{
	if (!m_IsInitialized)
	{
		LoadEffect(gameContext, m_effectFile);
		CreateVertexBuffer(gameContext);
		CreateIndexBuffer(gameContext);

		m_pRenderTarget = new RenderTarget(gameContext.pDevice);

		const auto gamesettings = OverlordGame::GetGameSettings();

		RENDERTARGET_DESC rtDesc;
		rtDesc.Width = gamesettings.Window.Width;
		rtDesc.Height = gamesettings.Window.Height;

		const auto result = m_pRenderTarget->Create(rtDesc);
		Logger::LogHResult(result, L"[PostProcessingMaterial] Initialize");
		m_IsInitialized = true;
	}
}

bool PostProcessingMaterial::LoadEffect(const GameContext& gameContext, const std::wstring& effectFile)
{
	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(effectFile);

	if (!m_TechniqueName.empty())
	{
		auto techString = std::string(m_TechniqueName.begin(), m_TechniqueName.end());
		m_pTechnique = m_pEffect->GetTechniqueByName(techString.c_str());
	}
	else
	{
		m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	}

	//Build InputLayout
	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, m_pInputLayoutDescriptions,
	                               m_pInputLayoutSize, m_InputLayoutID);

	LoadEffectVariables();

	return true;
}

void PostProcessingMaterial::Draw(const GameContext& gameContext, RenderTarget* previousRendertarget)
{
	m_pRenderTarget->Clear(gameContext, reinterpret_cast<const float*>(&DirectX::Colors::Pink));

	UpdateEffectVariables(previousRendertarget);

	//// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);


	//// Set vertex buffer
	UINT offset = 0;
	UINT stride = sizeof(VertexPosTex);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	//// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//// Draw
	D3DX11_TECHNIQUE_DESC techDesc{};
	(m_pTechnique)->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		(m_pTechnique)->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}

	// Generate Mips
	gameContext.pDeviceContext->GenerateMips(m_pRenderTarget->GetShaderResourceView());
}

void PostProcessingMaterial::CreateVertexBuffer(const GameContext& gameContext)
{
	m_NumVertices = 4;
	// Create vertex array containing three elements in system memory
	VertexPosTex* pVertices = new VertexPosTex[m_NumVertices];
	pVertices[0] = VertexPosTex(DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0, 1));
	pVertices[1] = VertexPosTex(DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1, 1));
	pVertices[2] = VertexPosTex(DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1, 0));
	pVertices[3] = VertexPosTex(DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0, 0));

	//fill a buffer description to copy the vertexdata into graphics memory
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VertexPosTex) * m_NumVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	//create a ID3D10Buffer in graphics meemory containing the vertex info
	const auto result = gameContext.pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	Logger::LogHResult(result, L"PostProcessingMaterial::CreateVertexBuffer");

	m_VertexBufferStride = sizeof( VertexPosTex);
	delete[] pVertices;
}

void PostProcessingMaterial::CreateIndexBuffer(const GameContext& gameContext)
{
	m_NumIndices = 6;
	// Create index buffer
	UINT indices[] =
	{
		0, 3, 1, 1, 3, 2,
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( UINT) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
	const auto result = (gameContext.pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer));
	Logger::LogHResult(result, L"PostProcessingMaterial::CreateIndexBuffer");
}
