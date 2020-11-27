#include "QScrollAreaTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScrollAreaTest w;
    w.show();
    return a.exec();
}
