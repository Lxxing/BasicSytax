#pragma once

#include <QWidget>
#include "ui_QtWidgetsItem.h"

class QtWidgetsItem : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsItem(QWidget *parent = Q_NULLPTR);
	~QtWidgetsItem();

private:
	Ui::QtWidgetsItem ui;
};
