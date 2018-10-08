#include "stdafx.h"
#include "OverlordEditor.h"
#include <QtWidgets/QApplication>
#include <dxgidebug.h>
#include "D3DRenderWidget.h"
#include "MainGame.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OverlordEditor w;
	//w.show();
	w.showMaximized();
	
	return a.exec();
}
