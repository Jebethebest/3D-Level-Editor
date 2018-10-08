#include "stdafx.h"
#include "MaterialManager.h"
#include "Material.h"

MaterialManager::MaterialManager():
	m_pMaterials(std::map<UINT, Material*>()),
	m_pMaterials_Deferred(std::map<UINT, Material*>())
{
}

MaterialManager::~MaterialManager()
{
	//Delete Materials
	for (std::pair<UINT, Material*> materialEntry : m_pMaterials)
	{
		SafeDelete(materialEntry.second);
	}

	m_pMaterials.clear();

	//Delete Deferred Materials
	for (std::pair<UINT, Material*> materialEntry : m_pMaterials_Deferred)
	{
		SafeDelete(materialEntry.second);
	}

	m_pMaterials_Deferred.clear();
}

Material* MaterialManager::GetMaterial(UINT materialId, bool forceDeferred) const
{

	if(m_RenderingContext == MaterialRenderingContext::DEFERRED_OPAQUE || forceDeferred)
	{
		const auto it = m_pMaterials_Deferred.find(materialId);
		if (it == m_pMaterials_Deferred.end())
		{
			Logger::LogFormat(LogLevel::Error, L"MaterialManager::GetMaterial() > DEFERRED Material with ID %i not found!", materialId);
			return nullptr;
		}

		return it->second;
	}
	else
	{
		const auto it = m_pMaterials.find(materialId);
		if (it == m_pMaterials.end())
		{
			Logger::LogFormat(LogLevel::Error, L"MaterialManager::GetMaterial() > Material with ID %i not found!", materialId);
			return nullptr;
		}

		return it->second;
	}
}

void MaterialManager::RemoveMaterial(UINT materialId)
{
	//FORWARD MATERIALS
	auto it = m_pMaterials.find(materialId);
	auto found = !(it == m_pMaterials.end());

	if (found)
	{
		SafeDelete(it->second);
		m_pMaterials.erase(it);
	}

	//DEFERRED MATERIALS
	it = m_pMaterials_Deferred.find(materialId);
	if (it == m_pMaterials_Deferred.end() && !found)
	{
		Logger::LogFormat(LogLevel::Warning, L"MaterialManager::RemoveMaterial() > Material OR Deferred Material with ID %i not found!", materialId);
		return;
	}

	SafeDelete(it->second);
	m_pMaterials_Deferred.erase(it);
}

void MaterialManager::AddMaterial(Material *pMaterial, UINT materialId)
{
#if _DEBUG
	if (pMaterial == nullptr)
	{
		Logger::LogError(L"MaterialManager::AddMaterial() > Material is NULL!");
		return;
	}

	for (std::pair<UINT, Material*> materialEntry : m_pMaterials)
	{
		if (materialEntry.first == materialId)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial() > MaterialID %i is already used!", materialId);
			return;
		}

		if (materialEntry.second == pMaterial)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial() > Material is already added to the manager (ID %i)!", materialId);
			return;
		}
	}
#endif

	m_pMaterials[materialId] = pMaterial;
}

void MaterialManager::AddMaterial(Material* pMaterial, Material* pMaterial_Deferred, UINT materialId)
{
	//Regular Material
	AddMaterial(pMaterial, materialId);

	//Deffered Material
#if _DEBUG
	if (pMaterial_Deferred == nullptr)
	{
		Logger::LogError(L"MaterialManager::AddMaterial()  [Deferred] > Material_Deferred is NULL!");
		return;
	}

	for (std::pair<UINT, Material*> materialEntry : m_pMaterials_Deferred)
	{
		if (materialEntry.first == materialId)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial() [Deferred] > MaterialID %i is already used!", materialId);
			return;
		}

		if (materialEntry.second == pMaterial_Deferred)
		{
			Logger::LogFormat(LogLevel::Warning, L"MaterialManager::AddMaterial()  [Deferred] > Material_Deferred is already added to the manager (ID %i)!", materialId);
			return;
		}
	}
#endif

	m_pMaterials_Deferred[materialId] = pMaterial_Deferred;
}
