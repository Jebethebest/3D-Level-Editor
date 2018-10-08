#include "stdafx.h"
#include "DeferredMaterial.h"

ID3DX11EffectShaderResourceVariable* DeferredMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* DeferredMaterial::m_pNormalSRVvariable = nullptr;

DeferredMaterial::DeferredMaterial() :
	Material(L"./Resources/Effects/Deferred/DeferredRenderer.fx")
{
}

void DeferredMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gTextureDiffuse")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DeferredMaterial::LoadEffectVariables() > \'gTextureDiffuse\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	if (!m_pNormalSRVvariable)
	{
		m_pNormalSRVvariable = GetEffect()->GetVariableByName("gTextureNormal")->AsShaderResource();
		if (!m_pNormalSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DeferredMaterial::LoadEffectVariables() > \'gTextureNormal\' variable not found!");
			m_pNormalSRVvariable = nullptr;
		}
	}
}

void DeferredMaterial::UpdateEffectVariables(const GameContext&, ModelComponent*) 
{
}

void DeferredMaterial::UpdateVariables() const
{
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseSRV);
	m_pNormalSRVvariable->SetResource(m_pNormalSRV);
}
