#include "stdafx.h"
#include "OverlordEditor.h"

#include "EditorScene.h"
#include <dxgidebug.h>
#include "MainGame.h"
#include "d3drenderwidget.h"
#include "SceneManager.h"
#include "Prefabs.h"
#include "Components.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include <QTimer>
#include <QFileDialog>
#include "ModelComponent.h"
#include <filesystem>


OverlordEditor::OverlordEditor(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{

	ui.setupUi(this);
	
	D3DRenderWidget* widget = new D3DRenderWidget(this);
	widget->CreateGame((HWND)ui.DrawWidget->winId());
	
	//widget->show();
	//ui.centralWidget->show();
	
	//auto pGame = new MainGame();
	//auto result = pGame->Run(GetModuleHandle(NULL), m_ID);
	//UNREFERENCED_PARAMETER(result);
	//delete pGame;
	
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(HideNShow()));
	timer->start(20);

	ui.ModelMaker->hide();
	ui.MaterialMaker->hide();
	ui.DeferredTexture->hide();
	ui.DeferredTextureBox->hide();
	
}

void OverlordEditor::HideNShow()
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	//Set correct values
	SetCorrectTransformValues(selectedObject);

	// Show Transform Window
	if (selectedObject && ui.Transform->isHidden())
	{
		ui.Transform->show();

		auto geometry = selectedObject->GetComponent<ColliderComponent>()->GetShape()->getGeometryType();

		if (geometry == physx::PxGeometryType::eSPHERE)
		{
			ui.ScaleY->hide();
			ui.ScaleZ->hide();
			return;
		}

		ui.ScaleY->show();
		ui.ScaleZ->show();
		return;
	}

	//Hide Transform Window
	if (!selectedObject && !ui.Transform->isHidden())
	{
		ui.Transform->hide();
		return;
	}
}

void OverlordEditor::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Level file"),"", 
													tr("Level Save (*.json);;All Files (*)"));
}

void OverlordEditor::on_PlaceModel_clicked()
{
	//ui.ModelBox->itemData(ui.ModelBox->currentIndex());
	auto pModel = m_pModelList[ui.ModelBox->currentIndex()];

	auto obj = new GameObject();
	obj->AddComponent(pModel);
	SceneManager::GetInstance()->GetActiveScene()->AddChild(obj);
}

void OverlordEditor::on_actionMaterial_triggered()
{
	ui.MaterialMaker->show();
}

#pragma region ACTIONS	

void OverlordEditor::NewScene()
{
}

void OverlordEditor::OpenScene()
{
}

void OverlordEditor::SaveScene()
{
}

void OverlordEditor::SaveSceneAs()
{
}

void OverlordEditor::Build()
{
}

void OverlordEditor::Undo()
{
}

void OverlordEditor::Redo()
{
}

void OverlordEditor::Cut()
{
}

void OverlordEditor::Copy()
{
}

void OverlordEditor::Paste()
{
}

void OverlordEditor::CreateActions()
{

}

#pragma endregion 

void OverlordEditor::on_ImportButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Import .ovm file"), "",
		tr("All Files (*);;OVM file (*.ovm);;PNG (*.png);;JPEG (*.jpg);;DDS (*.dds);;TGA (*.tga)"));
	std::wstring file = fileName.toStdWString();

	auto position = file.rfind(L"/");
	file.erase(0, position + 1);
	fileName = QString::fromStdWString(file);

	ui.listWidget->addItem(fileName);

	if (fileName.contains(".ovm"))
	{
		ui.ModelBox->addItem(fileName);
		m_pModelList.push_back(new ModelComponent(file));
	}

	else if (fileName.contains(".png") || fileName.contains(".jpg") ||
		fileName.contains(".tga") || fileName.contains(".dds"))
	{
		m_TextureList.push_back(file);
		ui.TextureBox->addItem(fileName);
	}
}

#pragma region SPAWN_OBJECT
void OverlordEditor::on_actionCube_triggered()
{
	auto cube1 = new CubePrefab();
	auto rigidbody = new RigidBodyComponent();
	rigidbody->SetKinematic(true);
	cube1->AddComponent(rigidbody);

	auto noBouncyMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0, 0, 0);

	std::shared_ptr<physx::PxGeometry> cubeGeom(new physx::PxBoxGeometry(0.5f, 0.5f, 0.5f));

	auto collider = new ColliderComponent(cubeGeom, *noBouncyMaterial, physx::PxTransform(physx::PxIdentity));
	cube1->AddComponent(collider);


	SceneManager::GetInstance()->GetActiveScene()->AddChild(cube1);
}

void OverlordEditor::on_actionSphere_triggered()
{
	auto sphere1 = new SpherePrefab();
	auto rigidBody = new RigidBodyComponent();
	rigidBody->SetKinematic(true);
	sphere1->AddComponent(rigidBody);

	auto noBouncyMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0, 0, 0);

	std::shared_ptr<physx::PxGeometry> sphereGeom(new physx::PxSphereGeometry(1.f));

	auto collider = new ColliderComponent(sphereGeom, *noBouncyMaterial, physx::PxTransform(physx::PxIdentity));
	sphere1->AddComponent(collider);

	SceneManager::GetInstance()->GetActiveScene()->AddChild(sphere1);
}

void OverlordEditor::on_actionModel_2_triggered()
{
	ui.ModelMaker->show();
}

void OverlordEditor::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	
}
#pragma endregion

#pragma region OBJECT_POSITION

void OverlordEditor::on_PositionX_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto position = selectedObject->GetTransform()->GetPosition();
		selectedObject->GetTransform()->Translate(arg1, position.y, position.z);
		return;
	}
	Logger::LogWarning(L"No Gameobject selected!");

}

void OverlordEditor::on_PositionY_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto position = selectedObject->GetTransform()->GetPosition();
		selectedObject->GetTransform()->Translate(position.x, arg1, position.z);
		return;
	}
	Logger::LogWarning(L"No Gameobject selected!");
}

void OverlordEditor::on_PositionZ_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto position = selectedObject->GetTransform()->GetPosition();
		selectedObject->GetTransform()->Translate(position.x, position.y, arg1);
		return;
	}
	Logger::LogWarning(L"No Gameobject selected!");
}

#pragma endregion

#pragma region OBJECT_ROTATION

void OverlordEditor::on_RotationX_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();
	SceneManager::GetInstance()->GetActiveScene();
	if(selectedObject)
	{
		auto rotation = selectedObject->GetTransform()->GetRotation();
		selectedObject->GetTransform()->Rotate(arg1, (rotation.y), (rotation.z), false);
		return;
	}
	Logger::LogWarning(L"No Gameobject selected!");
}

void OverlordEditor::on_RotationY_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto rotation = selectedObject->GetTransform()->GetRotation();
		selectedObject->GetTransform()->Rotate((rotation.x), DirectX::XMConvertToRadians(arg1), (rotation.z), false);
		return;
	}
	Logger::LogWarning(L"No Gameobject selected!");
}

void OverlordEditor::on_RotationZ_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto rotation = selectedObject->GetTransform()->GetWorldRotation();
		selectedObject->GetTransform()->Rotate(RadiansToDegrees(rotation.x), RadiansToDegrees(rotation.y), arg1);
		return;
	}
	Logger::LogWarning(L"No Gameobject selected!");
}

#pragma endregion 

#pragma region OBJECT_SCALING

void OverlordEditor::on_ScaleX_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto scale = selectedObject->GetTransform()->GetScale();
		selectedObject->GetTransform()->Scale(arg1, arg1, arg1);

		GetColliderScale(selectedObject, arg1);

		return;
	}
	Logger::LogWarning(L"No GameObject selected!");
}

void OverlordEditor::on_ScaleY_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto scale = selectedObject->GetTransform()->GetScale();
		selectedObject->GetTransform()->Scale(scale.x, arg1, scale.z);
		return;
	}
	Logger::LogWarning(L"No GameObject selected!");
}

void OverlordEditor::on_ScaleZ_valueChanged(double arg1)
{
	auto selectedObject = SceneManager::GetInstance()->GetActiveScene()->GetActiveGameObject();

	if (selectedObject)
	{
		auto scale = selectedObject->GetTransform()->GetScale();
		selectedObject->GetTransform()->Scale(scale.x, scale.y, arg1);
		return;
	}
	Logger::LogWarning(L"No GameObject selected!");
}
#pragma endregion 

#pragma region HELPER_FUNCS
void OverlordEditor::SetCorrectTransformValues(GameObject*& selectedObject)
{
	if (selectedObject)
	{
		auto position = selectedObject->GetTransform()->GetPosition();
		auto rotation = selectedObject->GetTransform()->GetRotation();
		auto scale = selectedObject->GetTransform()->GetScale();

		ui.PositionX->setValue(position.x);
		ui.PositionY->setValue(position.y);
		ui.PositionZ->setValue(position.z);

		//ui.RotationX->setValue(RadiansToDegrees(rotation.x));
		//ui.RotationY->setValue(RadiansToDegrees(rotation.y));
		//ui.RotationZ->setValue(RadiansToDegrees(rotation.z));

		Logger::LogWarning(std::to_wstring(RadiansToDegrees(rotation.x)));


		ui.ScaleX->setValue(scale.x);
		ui.ScaleY->setValue(scale.y);
		ui.ScaleZ->setValue(scale.z);
	}
}

void OverlordEditor::GetColliderScale(GameObject*& object, double scale)
{
	auto shape = object->GetComponent<ColliderComponent>()->GetShape()->getGeometryType();
	switch (shape)
	{
	case physx::PxGeometryType::eBOX:
		{
			auto geometry = object->GetComponent<ColliderComponent>()->GetShape()->getGeometry().box();
			object->GetComponent<ColliderComponent>()->GetShape()->getBoxGeometry(geometry);
			geometry.halfExtents.y += scale;
			object->GetComponent<ColliderComponent>()->GetShape()->setGeometry(geometry);
			break;
		}


	case physx::PxGeometryType::eSPHERE:
		{
			auto geometry = object->GetComponent<ColliderComponent>()->GetShape()->getGeometry().sphere();
			object->GetComponent<ColliderComponent>()->GetShape()->getSphereGeometry(geometry);

			geometry.radius += scale/2;
			object->GetComponent<ColliderComponent>()->GetShape()->setGeometry(geometry);

			break;
		}



	}
}

void OverlordEditor::GetAvailableMaterials()
{
	//for(const auto& path : std::experimental::filesystem::directory_iterator("..\OverlordProject\Materials"))
	//{
	//	Logger::LogInfo(path.path().filename());
	//}
}

float OverlordEditor::RadiansToDegrees(float radians)
{
	return DirectX::XMConvertToDegrees(radians);
}
#pragma endregion
