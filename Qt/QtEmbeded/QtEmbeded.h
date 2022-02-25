#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtEmbeded.h"

class QtEmbeded : public QWidget
{
    Q_OBJECT

public:
    QtEmbeded(QWidget *parent = Q_NULLPTR);
	void initQt();
	void initWin();
private:
    Ui::QtEmbededClass ui;

	WId calcID;
	QWindow * windowWnd;
	QWidget * childWdg;
};
