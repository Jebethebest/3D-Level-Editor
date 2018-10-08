#pragma once

#include <QWidget>
#include <QString>

class MainGame;
class D3DRenderWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(D3DRenderWidget)

public:
	D3DRenderWidget(QWidget *parent = Q_NULLPTR);
	virtual ~D3DRenderWidget();

	void CreateGame(HWND iD);

	void paintEvent(QPaintEvent* event);
	virtual QPaintEngine* paintEngine() const { return NULL; }
private:
	//Ui::D3DRenderWidget ui;

	MainGame *m_pMainGame = nullptr;
};
