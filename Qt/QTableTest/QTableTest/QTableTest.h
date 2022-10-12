#pragma once

#include <QtWidgets/QWidget>
#include "ui_QTableTest.h"

class QCheckBox;
class QTableTest : public QWidget
{
	Q_OBJECT

public:
	QTableTest(QWidget *parent = Q_NULLPTR);

	void init();

public slots:
	void on_btnALL_clicked();
private:
	Ui::QTableTestClass ui;

	QCheckBox * mCheckALL;
};
