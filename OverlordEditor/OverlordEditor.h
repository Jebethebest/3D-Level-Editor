#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OverlordEditor.h"

class GameObject;
class ModelComponent;

class OverlordEditor : public QMainWindow
{
	Q_OBJECT

public:
	OverlordEditor(QWidget *parent = Q_NULLPTR, Qt::WindowFlags = 0);

protected:
#ifndef QT_NO_CONTEXTMENU
	//void contextMenuEvent(QContextMenuEvent* event) override;
#endif

private slots:
	void HideNShow();

	void NewScene();
	void OpenScene();
	void SaveScene();
	void SaveSceneAs();
	void Build();
	void Undo();
	void Redo();
	void Cut();
	void Copy();
	void Paste();

	void on_actionCube_triggered();
	void on_actionSphere_triggered();
	void on_actionModel_2_triggered();
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

	void on_ImportButton_clicked();

	void on_PositionX_valueChanged(double arg1);
	void on_PositionY_valueChanged(double arg1);
	void on_PositionZ_valueChanged(double arg1);

	void on_RotationX_valueChanged(double arg1);
	void on_RotationY_valueChanged(double arg1);
	void on_RotationZ_valueChanged(double arg1);

	void on_ScaleX_valueChanged(double arg1);
	void on_ScaleY_valueChanged(double arg1);
	void on_ScaleZ_valueChanged(double arg1);

	void on_actionOpen_triggered();

	void on_PlaceModel_clicked();

	void on_actionMaterial_triggered();

private:
	void CreateActions();

	float RadiansToDegrees(float radians);

	void SetCorrectTransformValues(GameObject*& selectedObject);
	void GetColliderScale(GameObject *& object, double scale);
	void GetAvailableMaterials();

	Ui::OverlordEditorClass ui;
	
	QAction *m_pNewScene;

	std::vector<ModelComponent*> m_pModelList;
	std::vector<std::wstring> m_TextureList;
};
