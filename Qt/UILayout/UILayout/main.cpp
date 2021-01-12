#include "UILay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	UiLay w;
    w.show();
    return a.exec();
}
