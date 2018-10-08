#include "stdafx.h"
#include "ModelComponent.h"
#include "ContentManager.h"
#include "MeshFilter.h"
#include "Material.h"
#include "ModelAnimator.h"
#include "OverlordGame.h"

ModelComponent::ModelComponent(std::wstring assetFile):
	m_AssetFile(std::move(assetFile))
{
}

ModelComponent::~ModelComponent()
{
	SafeDelete(m_pAnimator);
}

void ModelComponent::Initialize(const GameContext& gameContext)
{
	m_pMeshFilter = ContentManager::Load<MeshFilter>(m_AssetFile);
	m_pMeshFilter->BuildIndexBuffer(gameContext);

	if (m_pMeshFilter->m_HasAnimations)
		m_pAnimator = new ModelAnimator(m_pMeshFilter);

	UpdateMaterial(gameContext);
};

inline void ModelComponent::UpdateMaterial(const GameContext& gameContext)
{
	if (m_MaterialSet)
	{
		m_MaterialSet = false;

		//FORWARD
		const auto mat = gameContext.pMaterialManager->GetMaterial(m_MaterialId);
		if (mat == nullptr)
		{
			Logger::LogFormat(LogLevel::Warning, L"ModelComponent::UpdateMaterial > Material with ID \"%i\" doesn't exist!",
			                  m_MaterialId);
			return;
		}

		m_pMaterial = mat;
		m_pMaterial->Initialize(gameContext);
		m_pMeshFilter->BuildVertexBuffer(gameContext, m_pMaterial);

		if (OverlordGame::GetGameSettings().DirectX.EnableDeferredRendering)
		{
			auto matDeferred = gameContext.pMaterialManager->GetMaterial(m_MaterialId, true);
			if (matDeferred == nullptr)
			{
				Logger::LogFormat(LogLevel::Warning,
				                  L"ModelComponent::UpdateMaterial > No Deferred Material with ID \"%i\"! [DEFERRED MODE: ON]",
				                  m_MaterialId);
				return;
			}

			matDeferred->Initialize(gameContext);
		}
	}
}

void ModelComponent::Update(const GameContext& gameContext)
{
	UpdateMaterial(gameContext);

	if (m_pAnimator)
		m_pAnimator->Update(gameContext);
};

void ModelComponent::Draw(const GameContext& gameContext)
{
	if (!m_pMaterial)
	{
		Logger::LogWarning(L"ModelComponent::Draw() > No Material!");
		return;
	}

	m_pMaterial = gameContext.pMaterialManager->GetMaterial(m_MaterialId);
	m_pMaterial->SetEffectVariables(gameContext, this);

	//Set Inputlayout
	gameContext.pDeviceContext->IASetInputLayout(m_pMaterial->GetInputLayout());

	//Set Vertex Buffer
	UINT offset = 0;
	auto vertexBufferData = m_pMeshFilter->GetVertexBufferData(gameContext, m_pMaterial);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride,
	                                               &offset);

	//Set Index Buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set Primitive Topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DRAW
	auto tech = m_pMaterial->GetTechnique();
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_pMeshFilter->m_IndexCount, 0, 0);
	}
};

void ModelComponent::SetMaterial(unsigned int materialId)
{
	m_MaterialSet = true;
	m_MaterialId = materialId;
}
