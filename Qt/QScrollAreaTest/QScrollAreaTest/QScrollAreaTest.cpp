#include "QScrollAreaTest.h"

#include "QtWidgetsItem.h"

#include <QGridLayout>

QScrollAreaTest::QScrollAreaTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void QScrollAreaTest::initItems()
{

	QGridLayout * gridLayout = new QGridLayout(ui.scrollAreaWidgetContents);
	for (int i = 0; i < 28; i++)
	{
		QtWidgetsItem * item = new QtWidgetsItem;
		gridLayout->setSpacing(10);
		gridLayout->addWidget(item,i / 5,i % 5);
	}
}
