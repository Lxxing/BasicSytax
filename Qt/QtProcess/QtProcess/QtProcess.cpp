#include "QtProcess.h"

#include <Windows.h>

#include <QProcess>
#include <QDebug>

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

void QtProcess::cmdTest()
{
	
	QStringList arguments;
	arguments << "/K";
	arguments << "title";
	arguments << "child process";

	QProcess *myProcess = new QProcess(this);
	//QString program = "D:/Work/Code/VsProj/BasicSytax/Qt/QtProcess/x64/Debug/ConsoleTest.exe";
	//QString program = "C:/Windows/System32/notepad.exe";
	QString program = "C:/Windows/System32/cmd.exe";
	myProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args)
	{
		//args->startupInfo->wShowWindow = SW_SHOWMAXIMIZED;//此成员变量设置此有效，QT5.12
		args->flags |= CREATE_NEW_CONSOLE;
		args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
		args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
		args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED
			| FOREGROUND_INTENSITY;
		//args->startupInfo->dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	}
	);
	myProcess->start(program, arguments);
	/*
	https://bugreports.qt.io/browse/QTBUG-57687,好多版本呢不在单独创建窗口
	bool rnt = myProcess->startDetached(program, arguments);
	if (true !=rnt)
	{
		qDebug() << "error";
	}*/
}
