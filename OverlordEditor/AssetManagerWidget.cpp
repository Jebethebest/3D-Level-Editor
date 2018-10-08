#include "stdafx.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>
#include <QString>
#include <QList>

#include "AssetManagerWidget.h"

AssetManagerWidget::AssetManagerWidget(QWidget *parent)
	: QListWidget(parent)
{
	setAcceptDrops(true);

	connect(this, SIGNAL(itemDoubleClicked(QListWidget*)), this, SLOT(onModelClicked(QListWidgetItem*)));
}

AssetManagerWidget::~AssetManagerWidget()
{
}

void AssetManagerWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if(event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void AssetManagerWidget::dropEvent(QDropEvent* event)
{
	Logger::LogError(L"Dropped!");
	for(const QUrl &url : event->mimeData()->urls())
	{
		QString fileName = url.toLocalFile();
		this->addItem(fileName);
		Logger::LogError(fileName.toStdWString());
	}
}
