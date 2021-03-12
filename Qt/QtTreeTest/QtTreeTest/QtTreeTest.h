#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtTreeTest.h"

class QTreeView;
class QtTreeTest : public QWidget
{
    Q_OBJECT

public:
    QtTreeTest(QWidget *parent = Q_NULLPTR);

	void init();
private:
    Ui::QtTreeTestClass ui;

	QTreeView *view;
	QMap<QString, QVector<QString>> origalData;
};
