#pragma once

struct RENDERTARGET_DESC
{
	int Width;
	int Height;
	ID3D11Texture2D* pColor;
	ID3D11Texture2D* pDepth;

	RENDERTARGET_DESC():
		Width(-1),
		Height(-1),
		pColor(nullptr),
		pDepth(nullptr)
	{}

	int IsValid() const
	{
		if(pColor == nullptr)
		{
			if(Width <= 0 || Height <= 0)
				return -1;
			return 0;
		}
		return 1;
	}
};

class RenderTarget
{
public:
	RenderTarget(ID3D11Device* pDevice);
	~RenderTarget();

	RenderTarget(const RenderTarget& other) = delete;
	RenderTarget(RenderTarget&& other) noexcept = delete;
	RenderTarget& operator=(const RenderTarget& other) = delete;
	RenderTarget& operator=(RenderTarget&& other) noexcept = delete;

	HRESULT Create(RENDERTARGET_DESC desc);
	void Clear(const GameContext& gameContext, const FLOAT colorRGBA[4]) const;

	ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView; }
	bool HasRenderTargetView() const { return m_pRenderTargetView != nullptr; }
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pColorShaderResourceView; }
	bool HasShaderResourceView() const { return m_pColorShaderResourceView != nullptr; }

private:
	
	ID3D11Device* m_pDevice;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11ShaderResourceView* m_pColorShaderResourceView;
	ID3D11Texture2D* m_pColor;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepth;

	HRESULT CreateColor(RENDERTARGET_DESC& desc);
	HRESULT CreateDepth(RENDERTARGET_DESC& desc);
	void CleanUp();

};

