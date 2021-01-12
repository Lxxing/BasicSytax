#ifndef VIDEODEVICE_H_
#define VIDEODEVICE_H_

extern "C" {
#include <libavcodec/avcodec.h>
#include "libavformat/avformat.h" 
#include "libswscale/swscale.h" 
#include "libavutil/avutil.h"
#include "libavutil/mem.h"
#include "libavutil/fifo.h"
}

#include <QObject>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <QList>

class VideoDevice :
	public QObject
{
public:
	explicit VideoDevice(QObject *parent = 0);
	virtual ~VideoDevice();

	struct ImgPacket {
		QImage PImage;
		struct ImgPacket *next;
	};
	int videoindex;
	int BUFFSIZE;
	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVPacket packet;
	AVFrame *pFrame, *pFrameRGB;
	char *FileName = "D://Code//VsProj//花开等花谢.mp4";
	QList<QImage> VideoImg;

signals:
	void SendImage(QImage img);

public slots:
	void Init();
	void Play();
};

#endif // VIDEODEVICE_H_