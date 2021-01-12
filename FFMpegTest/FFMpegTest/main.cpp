#include "FFMpegTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*printf("%s", avcodec_configuration());
	system("pause");*/
	FFMpegTest w;
	w.show();
	return a.exec();
}
