#include "stdafx.h"
#include "SpriteExComponent.h"
#include "GameObject.h"
#include "TextureData.h"
#include "ContentManager.h"
#include "SpriteRenderer.h"
#include "TransformComponent.h"

SpriteExComponent::SpriteExComponent(std::wstring spriteAsset, DirectX::XMFLOAT2 pivot, DirectX::XMFLOAT4 color):
	m_pTexture(nullptr),
	m_SpriteAsset(std::move(spriteAsset)),
	m_Pivot(pivot),
	m_Color(color)
{
}

void SpriteExComponent::Initialize(const GameContext& )
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteExComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteExComponent::Update(const GameContext& )
{
}

void SpriteExComponent::Draw(const GameContext& )
{
	if (!m_pTexture)
		return;

	SpriteVertex v;

#ifdef SPRITE_EFFECT_SUPPORT
	v.SpriteData.x = static_cast<float>(m_EffectType);
	v.SpriteData.y = m_EffectStart;
	v.SpriteData.z = m_EffectProgress;
	v.SpriteData.w = m_EffectParameter; //CUSTOM PER EFFECT
#endif

	const auto pos = GetTransform()->GetWorldPosition();
	const auto localPos = GetTransform()->GetPosition();
	const auto rot = GetTransform()->GetWorldRotation();
	const auto scale = GetTransform()->GetWorldScale();
	const float zRotation = QuaternionToEuler(rot).z;

	v.TransformData = DirectX::XMFLOAT4(pos.x, pos.y, localPos.z, zRotation);
	v.TransformData2 = DirectX::XMFLOAT4(m_Pivot.x, m_Pivot.y, scale.x, scale.y);
	v.Color = m_Color;

	SpriteRenderer::GetInstance()->Draw(m_pTexture, v);
}
