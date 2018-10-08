#include "stdafx.h"
#include "D3DRenderWidget.h"
#include "MainGame.h"
#include <dxgidebug.h>
#include <QTimer>
#include <QMessageBox>

D3DRenderWidget::D3DRenderWidget(QWidget *parent)
	: QWidget(parent)
{
	//setAttribute(Qt::WA_PaintOnScreen, true);
	//setAttribute(Qt::WA_NativeWindow, true);
	//ui.setupUi(this);

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(16);
}

D3DRenderWidget::~D3DRenderWidget()
{
	delete m_pMainGame;
}

void D3DRenderWidget::CreateGame(HWND iD)
{
	m_pMainGame = new MainGame();
	auto result = m_pMainGame->Run(GetModuleHandle(NULL), iD);
	UNREFERENCED_PARAMETER(result);

	QMessageBox::information(
		this,
		tr("Overlord Editor"),
		tr("For the moment, use right click to select a game object!"));
	//m_pMainGame->GameLoop();
	//delete m_pMainGame;

}

void D3DRenderWidget::paintEvent(QPaintEvent* event)
{
	m_pMainGame->GameLoop();
	Logger::LogInfo(L"Called Paint event!");
}
