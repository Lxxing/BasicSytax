#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtProcess.h"

class QtProcess : public QWidget
{
    Q_OBJECT

public:
    QtProcess(QWidget *parent = Q_NULLPTR);
	void init();
private:
    Ui::QtProcessClass ui;
};
