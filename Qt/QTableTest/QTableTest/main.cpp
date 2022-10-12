#include "QTableTest.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QString  qssStr = QApplication::applicationDirPath() + "/Table.qss";
	QFile f(qssStr);
	QString style;
	if (f.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
	{
		style = f.readAll().data();
		f.close();
		a.setStyleSheet(style);
	}
	QTableTest w;
	w.init();
	w.show();
	return a.exec();
}
