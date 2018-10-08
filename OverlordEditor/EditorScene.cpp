
#include "stdafx.h"
#include "EditorScene.h"

#include "Components.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "Prefabs.h"
#include "GameObject.h"
#include <QJsonArray>
#include "SceneManager.h"
#include <QJsonObject>

EditorScene::EditorScene() : GameScene(L"EditorScene") {}

void EditorScene::Initialize(const GameContext& gameContext)
{

}

void EditorScene::Update(const GameContext& gameContext)
{
	if(gameContext.pInput->IsMouseButtonDown(VK_RBUTTON))
	{
		std::cout << "Right click pressed!" << std::endl;
		m_pSelectedGameObject = gameContext.pCamera->Pick(gameContext);
		this->SetActiveGameObject(m_pSelectedGameObject);
	}

	//if (m_pSelectedGameObject)
	//	Logger::LogWarning(L"Position: " + std::to_wstring(m_pSelectedGameObject->GetTransform()->GetWorldPosition().x));
		//std::cout << m_pSelectedGameObject->GetTransform()->GetWorldPosition().x;
}

void EditorScene::Draw(const GameContext&)
{
	
}

void EditorScene::ReadLevel(const QJsonObject& json)
{
	
}

void EditorScene::WriteLevel(QJsonObject& json)
{
	QJsonArray objectArray;
	for(GameObject* object : SceneManager::GetInstance()->GetActiveScene()->GetSceneChildren())
	{

		
		QJsonObject jsonObject;
		WriteObject(jsonObject, object);

		objectArray.append(jsonObject);
	}
}

void EditorScene::ReadObject(const QJsonObject& json, GameObject*& object)
{

}

void EditorScene::WriteObject(QJsonObject& json, GameObject*& object)
{
	json["X Position"] = object->GetTransform()->GetPosition().x;
	json["Y Position"] = object->GetTransform()->GetPosition().y;
	json["Z Position"] = object->GetTransform()->GetPosition().z;

}