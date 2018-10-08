/********************************************************************************
** Form generated from reading UI file 'OverlordEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OVERLORDEDITOR_H
#define UI_OVERLORDEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "AssetManagerWidget.h"

QT_BEGIN_NAMESPACE

class Ui_OverlordEditorClass
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave_Scene;
    QAction *actionSave_Scene_as;
    QAction *actionBuild;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionDuplicate;
    QAction *actionDelete;
    QAction *actionInput;
    QAction *actionCreate;
    QAction *actionCreate_Empty;
    QAction *actionCube;
    QAction *actionSphere;
    QAction *actionText;
    QAction *actionNOPE;
    QAction *actionHelp;
    QAction *actionFor;
    QAction *actionYou;
    QAction *actionModel;
    QAction *actionMaterial;
    QWidget *centralWidget;
    QWidget *DrawWidget;
    QGroupBox *Inspector;
    QGroupBox *Transform;
    QLabel *Position;
    QDoubleSpinBox *PositionX;
    QDoubleSpinBox *PositionY;
    QDoubleSpinBox *PositionZ;
    QDoubleSpinBox *RotationY;
    QDoubleSpinBox *RotationX;
    QDoubleSpinBox *RotationZ;
    QLabel *Rotation;
    QDoubleSpinBox *ScaleX;
    QDoubleSpinBox *ScaleZ;
    QLabel *Scale;
    QDoubleSpinBox *ScaleY;
    QGroupBox *ModelMaker;
    QComboBox *MaterialBox;
    QComboBox *ModelBox;
    QLabel *Model;
    QLabel *Material;
    QLabel *Animation;
    QComboBox *AnimationBox;
    QPushButton *PlaceModel;
    QGroupBox *MaterialMaker;
    QComboBox *TextureBox;
    QComboBox *MaterialTypeBox;
    QLabel *MaterialType;
    QLabel *TextureSelection;
    QLabel *DeferredTexture;
    QComboBox *DeferredTextureBox;
    QPushButton *MakeMaterial;
    QGroupBox *AssetManager;
    AssetManagerWidget *listWidget;
    QPushButton *ImportButton;
    QGroupBox *Hierarchy;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuProject_Settings;
    QMenu *menuAssets;
    QMenu *menuGameObject;
    QMenu *menu3D_Object;
    QMenu *menu2D_Object;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OverlordEditorClass)
    {
        if (OverlordEditorClass->objectName().isEmpty())
            OverlordEditorClass->setObjectName(QStringLiteral("OverlordEditorClass"));
        OverlordEditorClass->setEnabled(true);
        OverlordEditorClass->resize(1920, 1080);
        OverlordEditorClass->setAcceptDrops(false);
        OverlordEditorClass->setWindowOpacity(1);
        OverlordEditorClass->setAutoFillBackground(true);
        OverlordEditorClass->setDockNestingEnabled(false);
        actionNew = new QAction(OverlordEditorClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(OverlordEditorClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave_Scene = new QAction(OverlordEditorClass);
        actionSave_Scene->setObjectName(QStringLiteral("actionSave_Scene"));
        actionSave_Scene_as = new QAction(OverlordEditorClass);
        actionSave_Scene_as->setObjectName(QStringLiteral("actionSave_Scene_as"));
        actionBuild = new QAction(OverlordEditorClass);
        actionBuild->setObjectName(QStringLiteral("actionBuild"));
        actionUndo = new QAction(OverlordEditorClass);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionRedo = new QAction(OverlordEditorClass);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionCut = new QAction(OverlordEditorClass);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        actionCopy = new QAction(OverlordEditorClass);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionPaste = new QAction(OverlordEditorClass);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        actionDuplicate = new QAction(OverlordEditorClass);
        actionDuplicate->setObjectName(QStringLiteral("actionDuplicate"));
        actionDelete = new QAction(OverlordEditorClass);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        actionInput = new QAction(OverlordEditorClass);
        actionInput->setObjectName(QStringLiteral("actionInput"));
        actionCreate = new QAction(OverlordEditorClass);
        actionCreate->setObjectName(QStringLiteral("actionCreate"));
        actionCreate_Empty = new QAction(OverlordEditorClass);
        actionCreate_Empty->setObjectName(QStringLiteral("actionCreate_Empty"));
        actionCube = new QAction(OverlordEditorClass);
        actionCube->setObjectName(QStringLiteral("actionCube"));
        actionSphere = new QAction(OverlordEditorClass);
        actionSphere->setObjectName(QStringLiteral("actionSphere"));
        actionText = new QAction(OverlordEditorClass);
        actionText->setObjectName(QStringLiteral("actionText"));
        actionNOPE = new QAction(OverlordEditorClass);
        actionNOPE->setObjectName(QStringLiteral("actionNOPE"));
        actionHelp = new QAction(OverlordEditorClass);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionFor = new QAction(OverlordEditorClass);
        actionFor->setObjectName(QStringLiteral("actionFor"));
        actionYou = new QAction(OverlordEditorClass);
        actionYou->setObjectName(QStringLiteral("actionYou"));
        actionModel = new QAction(OverlordEditorClass);
        actionModel->setObjectName(QStringLiteral("actionModel"));
        actionMaterial = new QAction(OverlordEditorClass);
        actionMaterial->setObjectName(QStringLiteral("actionMaterial"));
        centralWidget = new QWidget(OverlordEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        centralWidget->setAutoFillBackground(false);
        DrawWidget = new QWidget(centralWidget);
        DrawWidget->setObjectName(QStringLiteral("DrawWidget"));
        DrawWidget->setGeometry(QRect(320, 0, 1280, 720));
        DrawWidget->setMinimumSize(QSize(640, 480));
        Inspector = new QGroupBox(centralWidget);
        Inspector->setObjectName(QStringLiteral("Inspector"));
        Inspector->setGeometry(QRect(1599, -1, 321, 721));
        Inspector->setAutoFillBackground(true);
        Transform = new QGroupBox(Inspector);
        Transform->setObjectName(QStringLiteral("Transform"));
        Transform->setGeometry(QRect(9, 19, 301, 141));
        Position = new QLabel(Transform);
        Position->setObjectName(QStringLiteral("Position"));
        Position->setGeometry(QRect(20, 20, 71, 21));
        PositionX = new QDoubleSpinBox(Transform);
        PositionX->setObjectName(QStringLiteral("PositionX"));
        PositionX->setGeometry(QRect(90, 20, 62, 22));
        PositionX->setWrapping(false);
        PositionX->setAccelerated(true);
        PositionX->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        PositionX->setMinimum(-1000);
        PositionX->setMaximum(1000);
        PositionX->setSingleStep(0.5);
        PositionY = new QDoubleSpinBox(Transform);
        PositionY->setObjectName(QStringLiteral("PositionY"));
        PositionY->setGeometry(QRect(160, 20, 62, 22));
        PositionY->setWrapping(false);
        PositionY->setAccelerated(true);
        PositionY->setMinimum(-1000);
        PositionY->setMaximum(1000);
        PositionY->setSingleStep(0.5);
        PositionZ = new QDoubleSpinBox(Transform);
        PositionZ->setObjectName(QStringLiteral("PositionZ"));
        PositionZ->setGeometry(QRect(230, 20, 62, 22));
        PositionZ->setWrapping(false);
        PositionZ->setAccelerated(true);
        PositionZ->setMinimum(-1000);
        PositionZ->setMaximum(1000);
        PositionZ->setSingleStep(0.5);
        RotationY = new QDoubleSpinBox(Transform);
        RotationY->setObjectName(QStringLiteral("RotationY"));
        RotationY->setGeometry(QRect(160, 60, 62, 22));
        RotationY->setWrapping(false);
        RotationY->setAccelerated(true);
        RotationY->setDecimals(5);
        RotationY->setMinimum(-180);
        RotationY->setMaximum(180);
        RotationY->setSingleStep(1);
        RotationX = new QDoubleSpinBox(Transform);
        RotationX->setObjectName(QStringLiteral("RotationX"));
        RotationX->setGeometry(QRect(90, 60, 62, 22));
        RotationX->setWrapping(false);
        RotationX->setAccelerated(true);
        RotationX->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        RotationX->setDecimals(5);
        RotationX->setMinimum(-180);
        RotationX->setMaximum(180);
        RotationX->setSingleStep(1);
        RotationZ = new QDoubleSpinBox(Transform);
        RotationZ->setObjectName(QStringLiteral("RotationZ"));
        RotationZ->setGeometry(QRect(230, 60, 62, 22));
        RotationZ->setWrapping(false);
        RotationZ->setAccelerated(true);
        RotationZ->setDecimals(5);
        RotationZ->setMinimum(-180);
        RotationZ->setMaximum(180);
        RotationZ->setSingleStep(1);
        Rotation = new QLabel(Transform);
        Rotation->setObjectName(QStringLiteral("Rotation"));
        Rotation->setGeometry(QRect(20, 60, 71, 21));
        ScaleX = new QDoubleSpinBox(Transform);
        ScaleX->setObjectName(QStringLiteral("ScaleX"));
        ScaleX->setGeometry(QRect(90, 100, 62, 22));
        ScaleX->setWrapping(false);
        ScaleX->setAccelerated(true);
        ScaleX->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        ScaleX->setMinimum(0);
        ScaleX->setMaximum(1000);
        ScaleX->setSingleStep(1);
        ScaleX->setValue(1);
        ScaleZ = new QDoubleSpinBox(Transform);
        ScaleZ->setObjectName(QStringLiteral("ScaleZ"));
        ScaleZ->setGeometry(QRect(230, 100, 62, 22));
        ScaleZ->setWrapping(false);
        ScaleZ->setAccelerated(true);
        ScaleZ->setMinimum(0);
        ScaleZ->setMaximum(1000);
        ScaleZ->setSingleStep(1);
        ScaleZ->setValue(1);
        Scale = new QLabel(Transform);
        Scale->setObjectName(QStringLiteral("Scale"));
        Scale->setGeometry(QRect(20, 100, 71, 21));
        ScaleY = new QDoubleSpinBox(Transform);
        ScaleY->setObjectName(QStringLiteral("ScaleY"));
        ScaleY->setGeometry(QRect(160, 100, 62, 22));
        ScaleY->setWrapping(false);
        ScaleY->setAccelerated(true);
        ScaleY->setMinimum(0);
        ScaleY->setMaximum(1000);
        ScaleY->setSingleStep(1);
        ScaleY->setValue(1);
        ModelMaker = new QGroupBox(Inspector);
        ModelMaker->setObjectName(QStringLiteral("ModelMaker"));
        ModelMaker->setGeometry(QRect(10, 540, 301, 171));
        ModelMaker->setAutoFillBackground(true);
        MaterialBox = new QComboBox(ModelMaker);
        MaterialBox->setObjectName(QStringLiteral("MaterialBox"));
        MaterialBox->setGeometry(QRect(110, 60, 181, 22));
        ModelBox = new QComboBox(ModelMaker);
        ModelBox->setObjectName(QStringLiteral("ModelBox"));
        ModelBox->setGeometry(QRect(110, 30, 181, 22));
        Model = new QLabel(ModelMaker);
        Model->setObjectName(QStringLiteral("Model"));
        Model->setGeometry(QRect(20, 30, 91, 21));
        Material = new QLabel(ModelMaker);
        Material->setObjectName(QStringLiteral("Material"));
        Material->setGeometry(QRect(20, 60, 91, 21));
        Animation = new QLabel(ModelMaker);
        Animation->setObjectName(QStringLiteral("Animation"));
        Animation->setGeometry(QRect(20, 90, 91, 21));
        AnimationBox = new QComboBox(ModelMaker);
        AnimationBox->setObjectName(QStringLiteral("AnimationBox"));
        AnimationBox->setGeometry(QRect(110, 90, 181, 22));
        PlaceModel = new QPushButton(ModelMaker);
        PlaceModel->setObjectName(QStringLiteral("PlaceModel"));
        PlaceModel->setGeometry(QRect(110, 120, 93, 28));
        MaterialMaker = new QGroupBox(Inspector);
        MaterialMaker->setObjectName(QStringLiteral("MaterialMaker"));
        MaterialMaker->setGeometry(QRect(10, 360, 301, 171));
        MaterialMaker->setAutoFillBackground(true);
        TextureBox = new QComboBox(MaterialMaker);
        TextureBox->setObjectName(QStringLiteral("TextureBox"));
        TextureBox->setGeometry(QRect(110, 60, 181, 22));
        MaterialTypeBox = new QComboBox(MaterialMaker);
        MaterialTypeBox->setObjectName(QStringLiteral("MaterialTypeBox"));
        MaterialTypeBox->setGeometry(QRect(110, 30, 181, 22));
        MaterialType = new QLabel(MaterialMaker);
        MaterialType->setObjectName(QStringLiteral("MaterialType"));
        MaterialType->setGeometry(QRect(20, 30, 91, 21));
        TextureSelection = new QLabel(MaterialMaker);
        TextureSelection->setObjectName(QStringLiteral("TextureSelection"));
        TextureSelection->setGeometry(QRect(20, 60, 91, 21));
        DeferredTexture = new QLabel(MaterialMaker);
        DeferredTexture->setObjectName(QStringLiteral("DeferredTexture"));
        DeferredTexture->setGeometry(QRect(20, 90, 91, 21));
        DeferredTextureBox = new QComboBox(MaterialMaker);
        DeferredTextureBox->setObjectName(QStringLiteral("DeferredTextureBox"));
        DeferredTextureBox->setGeometry(QRect(110, 90, 181, 22));
        MakeMaterial = new QPushButton(MaterialMaker);
        MakeMaterial->setObjectName(QStringLiteral("MakeMaterial"));
        MakeMaterial->setGeometry(QRect(110, 120, 93, 28));
        AssetManager = new QGroupBox(centralWidget);
        AssetManager->setObjectName(QStringLiteral("AssetManager"));
        AssetManager->setGeometry(QRect(-1, 719, 1921, 291));
        AssetManager->setAutoFillBackground(true);
        listWidget = new AssetManagerWidget(AssetManager);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 50, 1901, 231));
        listWidget->setAcceptDrops(true);
        listWidget->setAutoFillBackground(true);
        listWidget->setFrameShape(QFrame::StyledPanel);
        listWidget->setFrameShadow(QFrame::Sunken);
        listWidget->setDragDropMode(QAbstractItemView::DragDrop);
        listWidget->setAlternatingRowColors(true);
        listWidget->setSortingEnabled(true);
        ImportButton = new QPushButton(AssetManager);
        ImportButton->setObjectName(QStringLiteral("ImportButton"));
        ImportButton->setGeometry(QRect(10, 20, 93, 28));
        Hierarchy = new QGroupBox(centralWidget);
        Hierarchy->setObjectName(QStringLiteral("Hierarchy"));
        Hierarchy->setGeometry(QRect(-1, -1, 321, 721));
        Hierarchy->setAutoFillBackground(true);
        OverlordEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(OverlordEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setEnabled(true);
        menuBar->setGeometry(QRect(0, 0, 1920, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuProject_Settings = new QMenu(menuEdit);
        menuProject_Settings->setObjectName(QStringLiteral("menuProject_Settings"));
        menuAssets = new QMenu(menuBar);
        menuAssets->setObjectName(QStringLiteral("menuAssets"));
        menuGameObject = new QMenu(menuBar);
        menuGameObject->setObjectName(QStringLiteral("menuGameObject"));
        menu3D_Object = new QMenu(menuGameObject);
        menu3D_Object->setObjectName(QStringLiteral("menu3D_Object"));
        menu2D_Object = new QMenu(menuGameObject);
        menu2D_Object->setObjectName(QStringLiteral("menu2D_Object"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        OverlordEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(OverlordEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setEnabled(true);
        OverlordEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(OverlordEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setEnabled(true);
        OverlordEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuAssets->menuAction());
        menuBar->addAction(menuGameObject->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Scene);
        menuFile->addAction(actionSave_Scene_as);
        menuFile->addSeparator();
        menuFile->addAction(actionBuild);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionDuplicate);
        menuEdit->addAction(actionDelete);
        menuEdit->addSeparator();
        menuEdit->addAction(menuProject_Settings->menuAction());
        menuProject_Settings->addAction(actionInput);
        menuAssets->addAction(actionCreate);
        menuGameObject->addAction(actionCreate_Empty);
        menuGameObject->addAction(menu3D_Object->menuAction());
        menuGameObject->addAction(menu2D_Object->menuAction());
        menuGameObject->addSeparator();
        menuGameObject->addAction(actionMaterial);
        menuGameObject->addAction(actionModel);
        menu3D_Object->addAction(actionCube);
        menu3D_Object->addAction(actionSphere);
        menu2D_Object->addAction(actionText);
        menuHelp->addAction(actionNOPE);
        menuHelp->addAction(actionHelp);
        menuHelp->addAction(actionFor);
        menuHelp->addAction(actionYou);

        retranslateUi(OverlordEditorClass);

        QMetaObject::connectSlotsByName(OverlordEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *OverlordEditorClass)
    {
        OverlordEditorClass->setWindowTitle(QApplication::translate("OverlordEditorClass", "OverlordEditor", nullptr));
        actionNew->setText(QApplication::translate("OverlordEditorClass", "New Scene", nullptr));
#ifndef QT_NO_STATUSTIP
        actionNew->setStatusTip(QApplication::translate("OverlordEditorClass", "Create a new Scene", nullptr));
#endif // QT_NO_STATUSTIP
        actionOpen->setText(QApplication::translate("OverlordEditorClass", "Open Scene", nullptr));
#ifndef QT_NO_STATUSTIP
        actionOpen->setStatusTip(QApplication::translate("OverlordEditorClass", "Open an existing Scene", nullptr));
#endif // QT_NO_STATUSTIP
        actionSave_Scene->setText(QApplication::translate("OverlordEditorClass", "Save Scene", nullptr));
#ifndef QT_NO_STATUSTIP
        actionSave_Scene->setStatusTip(QApplication::translate("OverlordEditorClass", "Save Current Scene", nullptr));
#endif // QT_NO_STATUSTIP
        actionSave_Scene_as->setText(QApplication::translate("OverlordEditorClass", "Save Scene as", nullptr));
#ifndef QT_NO_STATUSTIP
        actionSave_Scene_as->setStatusTip(QApplication::translate("OverlordEditorClass", "Save Current Scene as", nullptr));
#endif // QT_NO_STATUSTIP
        actionBuild->setText(QApplication::translate("OverlordEditorClass", "Build", nullptr));
        actionUndo->setText(QApplication::translate("OverlordEditorClass", "Undo", nullptr));
        actionRedo->setText(QApplication::translate("OverlordEditorClass", "Redo", nullptr));
        actionCut->setText(QApplication::translate("OverlordEditorClass", "Cut", nullptr));
        actionCopy->setText(QApplication::translate("OverlordEditorClass", "Copy", nullptr));
        actionPaste->setText(QApplication::translate("OverlordEditorClass", "Paste", nullptr));
        actionDuplicate->setText(QApplication::translate("OverlordEditorClass", "Duplicate", nullptr));
        actionDelete->setText(QApplication::translate("OverlordEditorClass", "Delete", nullptr));
        actionInput->setText(QApplication::translate("OverlordEditorClass", "Input", nullptr));
        actionCreate->setText(QApplication::translate("OverlordEditorClass", "Create", nullptr));
        actionCreate_Empty->setText(QApplication::translate("OverlordEditorClass", "Create Empty", nullptr));
        actionCube->setText(QApplication::translate("OverlordEditorClass", "Cube", nullptr));
        actionSphere->setText(QApplication::translate("OverlordEditorClass", "Sphere", nullptr));
        actionText->setText(QApplication::translate("OverlordEditorClass", "Text", nullptr));
        actionNOPE->setText(QApplication::translate("OverlordEditorClass", "No", nullptr));
        actionHelp->setText(QApplication::translate("OverlordEditorClass", "Help", nullptr));
        actionFor->setText(QApplication::translate("OverlordEditorClass", "For", nullptr));
        actionYou->setText(QApplication::translate("OverlordEditorClass", "You", nullptr));
        actionModel->setText(QApplication::translate("OverlordEditorClass", "Model", nullptr));
        actionMaterial->setText(QApplication::translate("OverlordEditorClass", "Material", nullptr));
        Inspector->setTitle(QApplication::translate("OverlordEditorClass", "Inspector", nullptr));
        Transform->setTitle(QApplication::translate("OverlordEditorClass", "Transform", nullptr));
        Position->setText(QApplication::translate("OverlordEditorClass", "Position", nullptr));
        PositionX->setPrefix(QString());
        PositionX->setSuffix(QString());
        PositionY->setPrefix(QString());
        PositionY->setSuffix(QString());
        PositionZ->setPrefix(QString());
        PositionZ->setSuffix(QString());
        RotationY->setPrefix(QString());
        RotationY->setSuffix(QString());
        RotationX->setPrefix(QString());
        RotationX->setSuffix(QString());
        RotationZ->setPrefix(QString());
        RotationZ->setSuffix(QString());
        Rotation->setText(QApplication::translate("OverlordEditorClass", "Rotation", nullptr));
        ScaleX->setPrefix(QString());
        ScaleX->setSuffix(QString());
        ScaleZ->setPrefix(QString());
        ScaleZ->setSuffix(QString());
        Scale->setText(QApplication::translate("OverlordEditorClass", "Scale", nullptr));
        ScaleY->setPrefix(QString());
        ScaleY->setSuffix(QString());
        ModelMaker->setTitle(QApplication::translate("OverlordEditorClass", "Model Maker", nullptr));
        Model->setText(QApplication::translate("OverlordEditorClass", "Model", nullptr));
        Material->setText(QApplication::translate("OverlordEditorClass", "Material", nullptr));
        Animation->setText(QApplication::translate("OverlordEditorClass", "Animation", nullptr));
        PlaceModel->setText(QApplication::translate("OverlordEditorClass", "Spawn Model", nullptr));
        MaterialMaker->setTitle(QApplication::translate("OverlordEditorClass", "Material Maker", nullptr));
        MaterialType->setText(QApplication::translate("OverlordEditorClass", "Type", nullptr));
        TextureSelection->setText(QApplication::translate("OverlordEditorClass", "Texture", nullptr));
        DeferredTexture->setText(QApplication::translate("OverlordEditorClass", "Def. Texture", nullptr));
        MakeMaterial->setText(QApplication::translate("OverlordEditorClass", "Make material", nullptr));
        AssetManager->setTitle(QApplication::translate("OverlordEditorClass", "Asset Manager", nullptr));
        ImportButton->setText(QApplication::translate("OverlordEditorClass", "Import", nullptr));
        Hierarchy->setTitle(QApplication::translate("OverlordEditorClass", "Hierarchy", nullptr));
        menuFile->setTitle(QApplication::translate("OverlordEditorClass", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("OverlordEditorClass", "Edit", nullptr));
        menuProject_Settings->setTitle(QApplication::translate("OverlordEditorClass", "Project Settings", nullptr));
        menuAssets->setTitle(QApplication::translate("OverlordEditorClass", "Assets", nullptr));
        menuGameObject->setTitle(QApplication::translate("OverlordEditorClass", "GameObject", nullptr));
        menu3D_Object->setTitle(QApplication::translate("OverlordEditorClass", "3D Object", nullptr));
        menu2D_Object->setTitle(QApplication::translate("OverlordEditorClass", "2D Object", nullptr));
        menuHelp->setTitle(QApplication::translate("OverlordEditorClass", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OverlordEditorClass: public Ui_OverlordEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OVERLORDEDITOR_H
