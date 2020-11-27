#include "QtWidgetsItem.h"

QtWidgetsItem::QtWidgetsItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_StyledBackground);
}

QtWidgetsItem::~QtWidgetsItem()
{
}
