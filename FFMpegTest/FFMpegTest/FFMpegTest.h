#ifndef FFMEPGTEST_H__
#define FFMEPGTEST_H__

#include <QThread>
#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QList>
#include <QTimer>
#include <QFile>
#include <QFileDialog>

#include "ui_FFMpegTest.h"
#include "VideoDevice.h"
class VideoDevice;
class FFMpegTest;
class FFMpegTest : public QWidget
{
	Q_OBJECT

public:
	explicit FFMpegTest(QWidget *parent = Q_NULLPTR);
	~FFMpegTest();

	QThread *VideoDecThread;
	VideoDevice *OVideoDec;
	QString FileName;
	QTimer *readTimer;
	int BUFFINDEX;

signals:
	void InitSignals();
	void PlaySignals();
public slots:
	void ShowVideo();
private slots:
	void OnVideoPlayClicked();

	void OnbtnOpenFileClicked();

private:
	FFMpegTest * ui;
};
#endif // FFMEPGTEST_H__