#include "QtTreeTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtTreeTest w;
    w.show();
    return a.exec();
}
