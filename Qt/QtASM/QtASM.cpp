#include "QtASM.h"
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>

bool checkPassword()
{
	bool result = true;
	QDateTime now = QDateTime::currentDateTime();
	QDateTime dummy = QDateTime::fromString("2022-11-10", "yyyy-MM-dd");

	qint64 testDateTime = now.secsTo(dummy);
	if (testDateTime < 0)//超过授权日期2022-11-10
	{
		result = false;
	}
	return result;
}


QtASM::QtASM(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [=]() {
		if (checkPassword())
		{
			ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime());
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("error.授权失败."));
			msgBox.exec();
		}

	});
	timer->start(1000);

}
