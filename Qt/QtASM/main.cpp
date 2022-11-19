#include "QtASM.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtASM w;
    w.show();
    return a.exec();
}
