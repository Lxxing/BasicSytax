#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtASM.h"

class QtASM : public QWidget
{
    Q_OBJECT

public:
    QtASM(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtASMClass ui;
};
