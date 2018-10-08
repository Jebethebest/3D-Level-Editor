#pragma once

class GameObject;
class SceneManager;
class CameraComponent;
class PhysxProxy;
class PostProcessingMaterial;

class GameScene
{
public:
	GameScene(std::wstring sceneName);
	GameScene(const GameScene& other) = delete;
	GameScene(GameScene&& other) noexcept = delete;
	GameScene& operator=(const GameScene& other) = delete;
	GameScene& operator=(GameScene&& other) noexcept = delete;
	virtual ~GameScene();

	void AddChild(GameObject* obj);
	void RemoveChild(GameObject* obj, bool deleteObject = true);
	const GameContext& GetGameContext() const { return m_GameContext; }

	PhysxProxy* GetPhysxProxy() const { return m_pPhysxProxy; }
	void SetActiveCamera(CameraComponent* pCameraComponent);

	// Update PP
	void AddPostProcessingEffect(PostProcessingMaterial* effect);
	void RemovePostProcessingEffect(PostProcessingMaterial* effect);

	//Editor
	GameObject*& GetActiveGameObject() {	return m_pActiveGameObject;	}
	void SetActiveGameObject(GameObject* pGameObject);

	std::vector<GameObject*> GetSceneChildren() { return m_pChildren; }

protected:

	virtual void Initialize(const GameContext& gameContext) = 0;
	virtual void Update(const GameContext& gameContext) = 0;
	virtual void Draw(const GameContext& gameContext) = 0;

	virtual void SceneActivated() {}
	virtual void SceneDeactivated() {}

	// Update PP
	std::vector<PostProcessingMaterial*> m_PostProcessingEffects;

private:

	friend class SceneManager;

	void RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void RootUpdate();
	void RootDraw();
	void RootSceneActivated();
	void RootSceneDeactivated();
	void RootWindowStateChanged(int state, bool active) const;

	std::vector<GameObject*> m_pChildren;
	GameContext m_GameContext;
	bool m_IsInitialized;
	std::wstring m_SceneName;
	CameraComponent *m_pDefaultCamera, *m_pActiveCamera;

	GameObject* m_pActiveGameObject = nullptr;

	// TODO: what if you have a game without physics? dependency should not exist here.
	PhysxProxy* m_pPhysxProxy;
};
