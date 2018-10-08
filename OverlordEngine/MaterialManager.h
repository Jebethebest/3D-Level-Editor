#pragma once
#include <map>

enum class MaterialRenderingContext
{
	FORWARD,
	DEFERRED_OPAQUE,
	DEFERRED_TRANSPARENT
};

class Material;

class MaterialManager final
{
public:
	MaterialManager(const MaterialManager& other) = delete;
	MaterialManager(MaterialManager&& other) noexcept = delete;
	MaterialManager& operator=(const MaterialManager& other) = delete;
	MaterialManager& operator=(MaterialManager&& other) noexcept = delete;
	MaterialManager();
	~MaterialManager();

	void AddMaterial(Material *pMaterial, UINT materialId);
	void AddMaterial(Material *pMaterial, Material* pMaterial_Deferred, UINT materialId);

	void RemoveMaterial(UINT materialId);
	Material* GetMaterial(UINT materialId, bool forceDeferred = false) const;

	MaterialRenderingContext GetRenderingContext() const { return m_RenderingContext; }
	void SetRenderingContext(MaterialRenderingContext context) { m_RenderingContext = context; }

private:

	std::map<UINT, Material*> m_pMaterials;
	std::map<UINT, Material*> m_pMaterials_Deferred;
	MaterialRenderingContext m_RenderingContext = MaterialRenderingContext::FORWARD;
};

