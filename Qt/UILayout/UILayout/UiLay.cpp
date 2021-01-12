#include "UiLay.h"
#include "ui_UiLay.h"

UiLay::UiLay(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::UiLay();
	ui->setupUi(this);
}

UiLay::~UiLay()
{
	delete ui;
}
