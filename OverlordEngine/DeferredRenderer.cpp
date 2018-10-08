#include "stdafx.h"
#include "DeferredRenderer.h"
#include "QuadDrawer.h"
#include "DeferredMaterial.h"
#include "OverlordGame.h"

DeferredRenderer::DeferredRenderer()
{
	for (auto i = 0; i < BUFFER_COUNT; i++)
	{
		m_pBuffers[i] = nullptr;
		m_pRenderTargetViews[i] = nullptr;
		m_pShaderResourceViews[i] = nullptr;
	}
}

DeferredRenderer::~DeferredRenderer()
{
	for (auto i = 0; i < BUFFER_COUNT; i++)
	{
		SafeRelease(m_pBuffers[i]);
		SafeRelease(m_pRenderTargetViews[i]);
		SafeRelease(m_pShaderResourceViews[i]);
	}

	SafeRelease(m_pDepthStencilBuffer);
	SafeRelease(m_pDepthStencilView);

	SafeDelete(m_pQuadDrawer);
	SafeDelete(m_pDeferredMat);
}

void DeferredRenderer::InitRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, OverlordGame* pGame)
{
	if (IsInitialized)
		Logger::LogWarning(L"DeferredRenderer::InitRenderer>> Already Initialized!!! (multiple invocations)");

	const auto gameSettings = OverlordGame::GetGameSettings();
	IsEnabled = gameSettings.DirectX.EnableDeferredRendering;

	if (!IsEnabled)
		Logger::LogWarning(L"DeferredRenderer::InitRenderer>> DeferredRendering DISABLED but initialized...");

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	m_pGame = pGame;

	const auto width = gameSettings.Window.Width;
	const auto height = gameSettings.Window.Height;

	//Create Buffers & Corresponding Views
	CreateBufferAndView(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, &m_pBuffers[int(DeferredBufferIDs::COLOR)],
	                    &m_pRenderTargetViews[int(DeferredBufferIDs::COLOR)],
	                    &m_pShaderResourceViews[int(DeferredBufferIDs::COLOR)]);
	CreateBufferAndView(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT, &m_pBuffers[int(DeferredBufferIDs::NORMAL)],
	                    &m_pRenderTargetViews[int(DeferredBufferIDs::NORMAL)],
	                    &m_pShaderResourceViews[int(DeferredBufferIDs::NORMAL)]);
	CreateBufferAndView(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, &m_pBuffers[int(DeferredBufferIDs::SPECULAR)],
	                    &m_pRenderTargetViews[int(DeferredBufferIDs::SPECULAR)],
	                    &m_pShaderResourceViews[int(DeferredBufferIDs::SPECULAR)]);
	CreateBufferAndView(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, &m_pBuffers[int(DeferredBufferIDs::LIGHT_ACCUMULATION)],
	                    &m_pRenderTargetViews[int(DeferredBufferIDs::LIGHT_ACCUMULATION)],
	                    &m_pShaderResourceViews[int(DeferredBufferIDs::LIGHT_ACCUMULATION)]);

	//Create Depth-Stencil Buffer and View
	CreateDepthStencilBufferAndView(width, height);

	//Initialize QuadDrawer & Material
	m_pQuadDrawer = new QuadDrawer();
	m_pQuadDrawer->Initialize(pDevice);

	m_pDeferredMat = new DeferredMaterial();

	GameContext tempContext {};
	tempContext.pDevice = pDevice;
	tempContext.pDeviceContext = pDeviceContext;
	m_pDeferredMat->Initialize(tempContext);
}

void DeferredRenderer::CreateBufferAndView(int width, int height, DXGI_FORMAT format, ID3D11Texture2D** pBuffer,
                                           ID3D11RenderTargetView** outputRTV,
                                           ID3D11ShaderResourceView** outputSRV) const
{
	//TEXTURE BUFFER
	//**************
	D3D11_TEXTURE2D_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	//Create Descriptor
	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.MipLevels = 1;
	bufferDesc.ArraySize = 1;
	bufferDesc.Format = format;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	//Create Resource
	auto hRes = m_pDevice->CreateTexture2D(&bufferDesc, nullptr, pBuffer);
	Logger::LogHResult(hRes, L"DeferredRenderer::CreateBuffer(...)"); //Blocking call on fail

	//RENDER TARGET VIEW
	//******************
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));

	//Create Descriptor
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	//Create Resource
	hRes = m_pDevice->CreateRenderTargetView(*pBuffer, &rtvDesc, outputRTV);
	Logger::LogHResult(hRes, L"DeferredRenderer::CreateBuffer(...)"); //Blocking call on fail

	//SHADER RESOURCE VIEW
	//********************
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	//Create Descriptor
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hRes = m_pDevice->CreateShaderResourceView(*pBuffer, &srvDesc, outputSRV);
	Logger::LogHResult(hRes, L"DeferredRenderer::CreateBuffer(...)"); //Blocking call on fail
}

void DeferredRenderer::CreateDepthStencilBufferAndView(int width, int height)
{
	//Texture Buffer
	//**************
	D3D11_TEXTURE2D_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	//Create Descriptor
	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.MipLevels = 1;
	bufferDesc.ArraySize = 1;
	bufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.SampleDesc.Quality = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	//Create Resource
	HRESULT hRes = m_pDevice->CreateTexture2D(&bufferDesc, nullptr, &m_pDepthStencilBuffer);
	Logger::LogHResult(hRes, L"DeferredRenderer::CreateDepthStencilBufferAndView(...)"); //Blocking call on fail


	//Depth Stencil View
	//******************
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//Create Descriptor
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create Resource
	hRes = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	Logger::LogHResult(hRes, L"DeferredRenderer::CreateDepthStencilBufferAndView(...)"); //Blocking call on fail
}

void DeferredRenderer::ClearRenderTargets(float r, float g, float b, float a)
{
	float clearColor[4] = {r, g, b, a};

	for (auto & m_pRenderTargetView : m_pRenderTargetViews)
	{
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	}

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeferredRenderer::SetRenderTargets() const
{
	m_pDeviceContext->OMSetRenderTargets(BUFFER_COUNT, m_pRenderTargetViews, m_pDepthStencilView);
}

void DeferredRenderer::Begin()
{
	//Hijack Rendering Pipeline :)
	ClearRenderTargets(0, 0, 0, 0);
	SetRenderTargets();
}

void DeferredRenderer::End()
{
	//Reset Default Rendertarget (Forward Rendering)
	ID3D11RenderTargetView* rTargets[4] = {nullptr, nullptr, nullptr, nullptr};
	m_pDeviceContext->OMSetRenderTargets(4, rTargets, nullptr);
	m_pGame->SetRenderTarget(nullptr);

	//Draw Final Composition
	m_pDeferredMat->SetDiffuseSRV(m_pShaderResourceViews[int(DeferredBufferIDs::COLOR)]);
	m_pDeferredMat->SetNormalSRV(m_pShaderResourceViews[int(DeferredBufferIDs::NORMAL)]);
	m_pDeferredMat->UpdateVariables();

	m_pQuadDrawer->DrawImmediate(m_pDeviceContext, m_pDeferredMat);

	ID3D11ShaderResourceView*const pSRV[2] = {nullptr, nullptr};
	m_pDeviceContext->PSSetShaderResources(0, 2, pSRV);
}
