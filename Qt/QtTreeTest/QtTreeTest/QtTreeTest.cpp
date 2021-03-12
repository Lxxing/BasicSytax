#include "QtTreeTest.h"

#include <QTreeView>

QtTreeTest::QtTreeTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	QVector<QString> item;
	item.append("a1");
	item.append("ѧУA1");
	origalData.insert("A1", item);

	item.append("a2");
	item.append("ѧУA2");
	origalData.insert("A2", item);

	item.append("a3");
	item.append("ѧУA3");
	origalData.insert("A3", item);

	item.append("a101");
	item.append("�༶A101");
	origalData.insert("A101", item);

	item.append("a102");
	item.append("�༶A102");
	origalData.insert("A102", item);

	item.append("a103");
	item.append("�༶A103");
	origalData.insert("A103", item);

	item.append("a111");
	item.append("�༶A111");
	origalData.insert("A111", item);

	item.append("a10101");
	item.append("��A10101");
	origalData.insert("A10101", item);

	item.append("a10201");
	item.append("��A10201");
	origalData.insert("A10201", item);

	view = new QTreeView(this);

}

void QtTreeTest::init()
{

}
