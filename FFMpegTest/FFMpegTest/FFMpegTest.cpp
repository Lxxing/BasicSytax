#include "FFMpegTest.h"
#include <QDebug> 
#include <QThread>

FFMpegTest::FFMpegTest(QWidget *parent)
	: QWidget(parent),
	ui(new FFMpegTest)
{
	ui->setupUi(this);
	
	VideoDecThread = new QThread(this);
	OVideoDec = new VideoDevice;
	readTimer = new QTimer(this);
	BUFFINDEX = 0;
	OVideoDec->moveToThread(VideoDecThread);
	VideoDecThread->start();
	connect(this, SIGNAL(InitSignals()), OVideoDec, SLOT(Init()));
	connect(this, SIGNAL(PlaySignals()), OVideoDec, SLOT(Play()));
	connect(OVideoDec,SIGNAL(SendImage(QImage)),this,SLOT(ShowVideo(QImage)));
	connect(readTimer, SIGNAL(timeout()), this, SLOT(ShowVideo()));
}

FFMpegTest::~FFMpegTest()
{
	delete ui;
}

void FFMpegTest::ShowVideo()
{
	emit InitSignals();
	readTimer->start(35);
}

void FFMpegTest::OnVideoPlayClicked()
{
	QPixmap pix;
	if (OVideoDec->VideoImg.isEmpty()) return;
	pix = pix.fromImage(OVideoDec->VideoImg.at(BUFFINDEX));
	//.scaledToWidth(ui->VideoShow->width());
	//ui->VideoShow->setPixmap(pix);
	if (BUFFINDEX > 50) {
		BUFFINDEX = 0;
		OVideoDec->BUFFSIZE = 0;
		OVideoDec->VideoImg.clear();
		return;
	}
	BUFFINDEX++;
}

void FFMpegTest::OnbtnOpenFileClicked()
{
	FileName = QFileDialog::getOpenFileName(this, "Open Video", "", "video file(*.mp4 *.rmvb)");
	QByteArray byte = FileName.toLocal8Bit();
	OVideoDec->FileName = byte.data();
}

