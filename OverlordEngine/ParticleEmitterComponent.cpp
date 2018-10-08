#include "stdafx.h"
#include "ParticleEmitterComponent.h"
 #include <utility>
#include "EffectHelper.h"
#include "ContentManager.h"
#include "TextureDataLoader.h"
#include "Particle.h"
#include "TransformComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent(std::wstring  assetFile, int particleCount):
	m_pVertexBuffer(nullptr),
	m_pEffect(nullptr),
	m_pParticleTexture(nullptr),
	m_pInputLayout(nullptr),
	m_pInputLayoutSize(0),
	m_Settings(ParticleEmitterSettings()),
	m_ParticleCount(particleCount),
	m_ActiveParticles(0),
	m_LastParticleInit(0.0f),
	m_AssetFile(std::move(assetFile))
{
	for (int i = 0; i < m_ParticleCount; ++i)
	{
		// TODO: this is an incorrect implementation of a particle pool! The particles are allocated all over the place in memory!
		m_Particles.push_back(new Particle(m_Settings));
	}
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	for (int i = 0; i < m_ParticleCount; ++i)
		SafeDelete(m_Particles[i]);

	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	LoadEffect(gameContext);

	CreateVertexBuffer(gameContext);

	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ParticleRenderer.fx");
	m_pDefaultTechnique = m_pEffect->GetTechniqueByIndex(0);

	m_pWvpVariable = m_pEffect->GetVariableByName("matWvp")->AsMatrix();
	m_pViewInverseVariable = m_pEffect->GetVariableByName("matViewInverse")->AsMatrix();
	m_pTextureVariable = m_pEffect->GetVariableByName("particleTexture")->AsShaderResource();

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	if (m_pVertexBuffer)
		SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ParticleVertex) * m_ParticleCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	const auto hr = gameContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"ParticleEmitterComponent::CreateVertexBuffer() > Buffer creation failed!");
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	const auto particleInterval = (m_Settings.MaxEnergy - m_Settings.MinEnergy) / m_ParticleCount;
	m_LastParticleInit += gameContext.pGameTime->GetElapsed();

	m_ActiveParticles = 0;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	const auto pBuffer = static_cast<ParticleVertex*>(mappedResource.pData);

	for (auto particle : m_Particles)
	{
		particle->Update(gameContext);

		if (particle->IsActive())
		{
			pBuffer[m_ActiveParticles] = particle->GetVertexInfo();
			++m_ActiveParticles;
		}
		else if (m_LastParticleInit >= particleInterval)
		{
			particle->Init(GetTransform()->GetWorldPosition());
			pBuffer[m_ActiveParticles] = particle->GetVertexInfo();
			++m_ActiveParticles;
			m_LastParticleInit = 0;
		}
	}

	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& )
{
}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	m_pWvpVariable->SetMatrix((float*)&gameContext.pCamera->GetViewProjection());
	m_pViewInverseVariable->SetMatrix((float*)&gameContext.pCamera->GetViewInverse());
	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	unsigned int offset = 0;
	const auto stride = sizeof(ParticleVertex);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		m_pDefaultTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}
