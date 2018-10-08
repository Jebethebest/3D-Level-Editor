#include "stdafx.h"
#include "Particle.h"

// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
using namespace DirectX;

Particle::Particle(const ParticleEmitterSettings& emitterSettings):
	m_VertexInfo(ParticleVertex()),
	m_EmitterSettings(emitterSettings),
	m_IsActive(false),
	m_TotalEnergy(0),
	m_CurrentEnergy(0),
	m_SizeGrow(0),
	m_InitSize(0)
{
}

void Particle::Update(const GameContext& gameContext)
{
	if (!m_IsActive)
		return;

	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if (m_CurrentEnergy < 0)
	{
		m_IsActive = false;
		return;
	}


	const float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;

	const auto emitterVelocity = XMLoadFloat3(&m_EmitterSettings.Velocity);
	auto vertPos = XMLoadFloat3(&m_VertexInfo.Position);
	vertPos += emitterVelocity * gameContext.pGameTime->GetElapsed();
	XMStoreFloat3(&m_VertexInfo.Position, vertPos);
	m_VertexInfo.Color = m_EmitterSettings.Color;
	m_VertexInfo.Color.w = particleLifePercent * 2;

	if (m_SizeGrow < 1)
		m_VertexInfo.Size = m_InitSize - m_InitSize * (m_SizeGrow * (1 - particleLifePercent));
	else if (m_SizeGrow > 1)
		m_VertexInfo.Size = m_InitSize + m_InitSize * (m_SizeGrow * (1 - particleLifePercent));
}

void Particle::Init(XMFLOAT3 initPosition)
{
	m_IsActive = true;

	//************
	// INIT ENERGY

	m_TotalEnergy = m_CurrentEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);


	//*************
	//INIT POSITION

	//Random direction, (unit std::vector (Y up) > transformed by random rotation matrix)
	auto randomDirection = XMVectorSet(0, 1, 0, 0);

	//Create random rotation matrix
	const auto randomRotationMatrix = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI),
	                                                             randF(-XM_PI, XM_PI));

	//Transform unit std::vector with random rotation matrix
	randomDirection = XMVector3TransformNormal(randomDirection, randomRotationMatrix);

	//Distance
	float randomDistance = randF(m_EmitterSettings.MinEmitterRange,
	                             m_EmitterSettings.MaxEmitterRange - m_EmitterSettings.MinEmitterRange);
	randomDistance += m_EmitterSettings.MinEmitterRange;

	//Position =>
	auto vertPos = XMLoadFloat3(&initPosition);
	vertPos += randomDirection * randomDistance;
	XMStoreFloat3(&m_VertexInfo.Position, vertPos);

	//*********
	//INIT SIZE

	m_VertexInfo.Size = m_InitSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);

	//********
	//ROTATION
	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);
}
