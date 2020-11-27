#pragma once

#include <QtWidgets/QWidget>
#include "ui_QScrollAreaTest.h"

class QScrollAreaTest : public QWidget
{
    Q_OBJECT

public:
    QScrollAreaTest(QWidget *parent = Q_NULLPTR);

	void initItems();
private:
    Ui::QScrollAreaTestClass ui;
};
