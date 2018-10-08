#include "stdafx.h"
#include "GameScene.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "OverlordGame.h"
#include "Prefabs.h"
#include "Components.h"
#include "DebugRenderer.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "RenderTarget.h"
#include "PhysxProxy.h"
#include "PostProcessingMaterial.h"
#include "SoundManager.h"
#include <algorithm>

GameScene::GameScene(std::wstring sceneName):
	m_pChildren(std::vector<GameObject*>()),
	m_GameContext(GameContext()),
	m_IsInitialized(false),
	m_SceneName(std::move(sceneName)),
	m_pDefaultCamera(nullptr),
	m_pActiveCamera(nullptr),
	m_pPhysxProxy(nullptr)
{
}

GameScene::~GameScene()
{
	SafeDelete(m_GameContext.pGameTime);
	SafeDelete(m_GameContext.pInput);
	SafeDelete(m_GameContext.pMaterialManager);

	for (auto pChild : m_pChildren)
	{
		SafeDelete(pChild);
	}
	SafeDelete(m_pPhysxProxy);

	// Update PP
	for (auto it = m_PostProcessingEffects.begin(); it != m_PostProcessingEffects.end(); ++it)
	{
		delete *it;
	}
	m_PostProcessingEffects.clear();
}

void GameScene::AddChild(GameObject* obj)
{
#if _DEBUG
	if (obj->m_pParentScene)
	{
		if (obj->m_pParentScene == this)
			Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to this GameScene");
		else
			Logger::LogWarning(
				L"GameScene::AddChild > GameObject is already attached to another GameScene. Detach it from it's current scene before attaching it to another one.");

		return;
	}

	if (obj->m_pParentObject)
	{
		Logger::LogWarning(
			L"GameScene::AddChild > GameObject is currently attached to a GameObject. Detach it from it's current parent before attaching it to another one.");
		return;
	}
#endif

	obj->m_pParentScene = this;
	obj->RootInitialize(m_GameContext);
	m_pChildren.push_back(obj);
}

void GameScene::RemoveChild(GameObject* obj, bool deleteObject)
{
	const auto it = find(m_pChildren.begin(), m_pChildren.end(), obj);

#if _DEBUG
	if (it == m_pChildren.end())
	{
		Logger::LogWarning(L"GameScene::RemoveChild > GameObject to remove is not attached to this GameScene!");
		return;
	}
#endif

	m_pChildren.erase(it);
	if (deleteObject)
	{
		delete obj;
		obj = nullptr;
	}
	else
		obj->m_pParentScene = nullptr;
}

// Update PP
void GameScene::AddPostProcessingEffect(PostProcessingMaterial* effect)
{
	if (find(m_PostProcessingEffects.begin(), m_PostProcessingEffects.end(), effect) == m_PostProcessingEffects.end())
	{
		m_PostProcessingEffects.push_back(effect);
		if (m_IsInitialized)
			effect->Initialize(m_GameContext);
	}
}

void GameScene::RemovePostProcessingEffect(PostProcessingMaterial* effect)
{
	m_PostProcessingEffects.erase(find(m_PostProcessingEffects.begin(), m_PostProcessingEffects.end(), effect));
	SafeDelete(effect);
}


void GameScene::SetActiveGameObject(GameObject* pGameObject)
{
	m_pActiveGameObject = pGameObject;
}

void GameScene::RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if (m_IsInitialized)
		return;

	//Create DefaultCamera
	auto freeCam = new FreeCamera();
	freeCam->SetRotation(30, 0);
	freeCam->GetTransform()->Translate(0, 50, -80);
	AddChild(freeCam);
	m_pDefaultCamera = freeCam->GetComponent<CameraComponent>();
	m_pActiveCamera = m_pDefaultCamera;
	m_GameContext.pCamera = m_pDefaultCamera;

	//Create GameContext
	m_GameContext.pGameTime = new GameTime();
	m_GameContext.pGameTime->Reset();
	m_GameContext.pGameTime->Stop();

	m_GameContext.pInput = new InputManager();
	InputManager::Initialize();

	m_GameContext.pMaterialManager = new MaterialManager();

	m_GameContext.pDevice = pDevice;
	m_GameContext.pDeviceContext = pDeviceContext;

	// Initialize Physx
	m_pPhysxProxy = new PhysxProxy();
	m_pPhysxProxy->Initialize(this);

	//User-Scene Initialize
	Initialize(m_GameContext);

	//Root-Scene Initialize
	for (auto pChild : m_pChildren)
	{
		pChild->RootInitialize(m_GameContext);
	}

	// Update PP
	for (auto effect : m_PostProcessingEffects)
	{
		effect->Initialize(m_GameContext);
	}

	m_IsInitialized = true;
}

void GameScene::RootUpdate()
{
	m_GameContext.pGameTime->Update();
	m_GameContext.pInput->Update();
	m_GameContext.pCamera = m_pActiveCamera;

	SoundManager::GetInstance()->GetSystem()->update();

	//User-Scene Update
	Update(m_GameContext);

	//Root-Scene Update
	for (auto pChild : m_pChildren)
	{
		pChild->RootUpdate(m_GameContext);
	}

	// Update PP
	//Sort effects based on render index (render smaller numbers first
	sort(m_PostProcessingEffects.begin(), m_PostProcessingEffects.end(),
	     [](PostProcessingMaterial* a, PostProcessingMaterial* b) -> bool
	     {
		     return a->GetRenderIndex() < b->GetRenderIndex();
	     });

	m_pPhysxProxy->Update(m_GameContext);
}

void GameScene::RootDraw()
{
	//User-Scene Draw
	Draw(m_GameContext);

	//Object-Scene Draw
	for (auto pChild : m_pChildren)
	{
		pChild->RootDraw(m_GameContext);
	}

	//Object-Scene Post-Draw
	for (auto pChild : m_pChildren)
	{
		pChild->RootPostDraw(m_GameContext);
	}

	//Draw PhysX
	m_pPhysxProxy->Draw(m_GameContext);

	//Draw Debug Stuff
	DebugRenderer::Draw(m_GameContext);
	SpriteRenderer::GetInstance()->Draw(m_GameContext);
	TextRenderer::GetInstance()->Draw(m_GameContext);

	// Update PP
	if (!m_PostProcessingEffects.empty())
	{
		auto game = SceneManager::GetInstance()->GetGame();
		const auto initialRenderTarget = game->GetRenderTarget();
		auto previousRenderTarget = initialRenderTarget;

		////Run all postprocessing effects
		for (auto effect : m_PostProcessingEffects)
		{
			const auto rendertarget = effect->GetRenderTarget();
			game->SetRenderTarget(rendertarget);
			effect->Draw(m_GameContext, previousRenderTarget);
			previousRenderTarget = rendertarget;
		}

		game->SetRenderTarget(initialRenderTarget);

		SpriteRenderer::GetInstance()->DrawImmediate(m_GameContext, previousRenderTarget->GetShaderResourceView(),
		                                             DirectX::XMFLOAT2(0, 0));
	}
}

void GameScene::RootSceneActivated()
{
	//Start Timer
	m_GameContext.pGameTime->Start();
	SceneActivated();
}

void GameScene::RootSceneDeactivated()
{
	//Stop Timer
	m_GameContext.pGameTime->Stop();
	SceneDeactivated();
}

void GameScene::RootWindowStateChanged(int state, bool active) const
{
	//TIMER
	if (state == 0)
	{
		if (active)m_GameContext.pGameTime->Start();
		else m_GameContext.pGameTime->Stop();
	}
}

void GameScene::SetActiveCamera(CameraComponent* pCameraComponent)
{
	//nullptr for DefaultCamera

	if (m_pActiveCamera != nullptr)
		m_pActiveCamera->m_IsActive = false;

	m_pActiveCamera = (pCameraComponent) ? pCameraComponent : m_pDefaultCamera;
	m_pActiveCamera->m_IsActive = true;
}
