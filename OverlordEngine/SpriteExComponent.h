#pragma once
#include "BaseComponent.h"

// TODO why is this here?
//#define SPRITE_EFFECT_SUPPORT

class TextureData;

enum class SpriteExEffectType
{
	None = 0,
	RadialFill = 1,
	HorizontalFill = 2,
	VerticalFill = 3
};

class SpriteExComponent : public BaseComponent
{
	TextureData* m_pTexture;
	std::wstring m_SpriteAsset;
	DirectX::XMFLOAT2 m_Pivot;
	DirectX::XMFLOAT4 m_Color;

	float m_EffectStart = 0.0f;
	float m_EffectProgress = 1.0f;
	float m_EffectParameter = 1.0f;
	SpriteExEffectType m_EffectType = SpriteExEffectType::None;

public:
	SpriteExComponent(const SpriteExComponent& other) = delete;
	SpriteExComponent(SpriteExComponent&& other) noexcept = delete;
	SpriteExComponent& operator=(const SpriteExComponent& other) = delete;
	SpriteExComponent& operator=(SpriteExComponent&& other) noexcept = delete;
	SpriteExComponent(std::wstring spriteAsset, DirectX::XMFLOAT2 pivot = DirectX::XMFLOAT2(0, 0),
	                  DirectX::XMFLOAT4 color = static_cast<DirectX::XMFLOAT4>(DirectX::Colors::White));
	virtual ~SpriteExComponent() = default;

	DirectX::XMFLOAT2 GetPivot() const { return m_Pivot; }
	DirectX::XMFLOAT4 GetColor() const { return m_Color; }

	void SetPivot(DirectX::XMFLOAT2 pivot) { m_Pivot = pivot; }
	void SetColor(DirectX::XMFLOAT4 color) { m_Color = color; }
	void SetTexture(const std::wstring& spriteAsset);

	void SetEffectType(SpriteExEffectType effect) { m_EffectType = effect; }
	void SetEffectStart(float effectStart) { m_EffectStart = effectStart; };
	void SetEffectProgress(float effectProgress) { m_EffectProgress = effectProgress; };
	void SetEffectParameter(float effectParameter) { m_EffectParameter = effectParameter; }

	SpriteExEffectType GetEffectType() const { return m_EffectType; }
	float GetEffectStart() const { return m_EffectStart; }
	float GetEffectProgress() const { return m_EffectProgress; }
	float GetEffectParameter() const { return m_EffectParameter; }

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
};
