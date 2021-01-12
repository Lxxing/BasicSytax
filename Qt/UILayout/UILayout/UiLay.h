#pragma once

#include <QWidget>
namespace Ui { class UiLay; };

class UiLay : public QWidget
{
	Q_OBJECT

public:
	UiLay(QWidget *parent = Q_NULLPTR);
	~UiLay();

private:
	Ui::UiLay *ui;
};
