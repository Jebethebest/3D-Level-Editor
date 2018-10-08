#pragma once

#include <QListWidget>

class QDragEvent;
class AssetManagerWidget : public QListWidget
{
	Q_OBJECT

public:
	AssetManagerWidget(QWidget *parent);
	~AssetManagerWidget();

	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
};
