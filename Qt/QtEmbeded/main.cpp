#include "QtEmbeded.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtEmbeded w;

	w.initQt();
	//w.initWin();
    
	w.show();
    return a.exec();
}
