#include "QScrollAreaTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScrollAreaTest w;
	w.initItems();
    w.show();
    return a.exec();
}
