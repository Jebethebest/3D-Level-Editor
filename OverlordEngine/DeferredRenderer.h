#pragma once
#include "Singleton.h"

enum class DeferredBufferIDs
{
	COLOR = 0,
	NORMAL = 1,
	SPECULAR = 2,
	LIGHT_ACCUMULATION = 3
};

class OverlordGame;
class QuadDrawer;
class DeferredMaterial;

class DeferredRenderer : public Singleton<DeferredRenderer>
{
public:
	DeferredRenderer();
	virtual ~DeferredRenderer();
	DeferredRenderer(const DeferredRenderer& other) = delete;
	DeferredRenderer(DeferredRenderer&& other) noexcept = delete;
	DeferredRenderer& operator=(const DeferredRenderer& other) = delete;
	DeferredRenderer& operator=(DeferredRenderer&& other) noexcept = delete;

	void InitRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, OverlordGame* pGame);
	ID3D11ShaderResourceView* GetShaderResourceView(DeferredBufferIDs id) const { return m_pShaderResourceViews[int(id)]; }

	void Begin();
	void End();

private:
	bool IsEnabled = false;
	bool IsInitialized = false;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	OverlordGame* m_pGame = nullptr;

	//Geometric Buffer (GBuffer)
	void CreateBufferAndView(int width, int height, DXGI_FORMAT format, ID3D11Texture2D** outputBuffer,
	                         ID3D11RenderTargetView** outputRTview = nullptr,
	                         ID3D11ShaderResourceView** outputSRview = nullptr) const;
	void CreateDepthStencilBufferAndView(int width, int height);
	void ClearRenderTargets(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void SetRenderTargets() const;

	static const int BUFFER_COUNT = 4;
	ID3D11Texture2D* m_pBuffers[BUFFER_COUNT]{};
	ID3D11RenderTargetView* m_pRenderTargetViews[BUFFER_COUNT]{};
	ID3D11ShaderResourceView* m_pShaderResourceViews[BUFFER_COUNT]{};
	ID3D11Texture2D* m_pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

	QuadDrawer* m_pQuadDrawer = nullptr;
	DeferredMaterial* m_pDeferredMat = nullptr;
};
