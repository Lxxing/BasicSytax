#include "QtProcess.h"

#include <QProcess>

QtProcess::QtProcess(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void QtProcess::init()
{
	QString program = "analogclock.exe";
	QStringList arguments;
	arguments << "-style" << "fusion";

	QProcess *myProcess = new QProcess(this);
	myProcess->start(program, arguments);
	bool rnt = myProcess->waitForStarted(30000);
}
