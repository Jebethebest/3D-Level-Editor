#pragma once
#include "GameScene.h"

class QJsonObject;
class GameObject;
class EditorScene : public GameScene
{
public:
	EditorScene();
	virtual ~EditorScene() = default;

	EditorScene(const EditorScene& other) = delete;
	EditorScene(EditorScene&& other) noexcept = delete;
	EditorScene& operator=(const EditorScene& other) = delete;
	EditorScene& operator=(EditorScene&& other) noexcept = delete;

	GameObject* GetSelectedGameObject() { return m_pSelectedGameObject; }

	void ReadLevel(const QJsonObject &json);
	void WriteLevel(QJsonObject &json);

	void ReadObject(const QJsonObject &json, GameObject*& object);
	void WriteObject(QJsonObject &json, GameObject*& object);

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:
	GameObject * m_pSelectedGameObject = nullptr;
};


