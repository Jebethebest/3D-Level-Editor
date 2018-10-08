#pragma once
#include "Material.h"

class DeferredMaterial : public Material
{
public:
	DeferredMaterial();
	virtual ~DeferredMaterial() = default;
	DeferredMaterial(const DeferredMaterial& other) = delete;
	DeferredMaterial(DeferredMaterial&& other) noexcept = delete;
	DeferredMaterial& operator=(const DeferredMaterial& other) = delete;
	DeferredMaterial& operator=(DeferredMaterial&& other) noexcept = delete;

	void SetDiffuseSRV(ID3D11ShaderResourceView* pDiffuseSRV) { m_pDiffuseSRV = pDiffuseSRV; }
	void SetNormalSRV(ID3D11ShaderResourceView* pNormalSRV) { m_pNormalSRV = pNormalSRV; }
	void UpdateVariables() const;

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:

	ID3D11ShaderResourceView* m_pDiffuseSRV = nullptr;
	ID3D11ShaderResourceView* m_pNormalSRV = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	static ID3DX11EffectShaderResourceVariable* m_pNormalSRVvariable;
};
