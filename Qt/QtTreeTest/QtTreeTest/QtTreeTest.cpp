#include "QtTreeTest.h"

#include <QTreeView>

QtTreeTest::QtTreeTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	QVector<QString> item;
	item.append("a1");
	item.append("学校A1");
	origalData.insert("A1", item);

	item.append("a2");
	item.append("学校A2");
	origalData.insert("A2", item);

	item.append("a3");
	item.append("学校A3");
	origalData.insert("A3", item);

	item.append("a101");
	item.append("班级A101");
	origalData.insert("A101", item);

	item.append("a102");
	item.append("班级A102");
	origalData.insert("A102", item);

	item.append("a103");
	item.append("班级A103");
	origalData.insert("A103", item);

	item.append("a111");
	item.append("班级A111");
	origalData.insert("A111", item);

	item.append("a10101");
	item.append("组A10101");
	origalData.insert("A10101", item);

	item.append("a10201");
	item.append("组A10201");
	origalData.insert("A10201", item);

	view = new QTreeView(this);

}

void QtTreeTest::init()
{

}
