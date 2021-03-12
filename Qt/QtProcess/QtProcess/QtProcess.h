#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtProcess.h"

class QtProcess : public QWidget
{
    Q_OBJECT

public:
    QtProcess(QWidget *parent = Q_NULLPTR);
	void init();//À­Æð²âÊÔ³ÌÐò
	void cmdTest();
private:
    Ui::QtProcessClass ui;
};
