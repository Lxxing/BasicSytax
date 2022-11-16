# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析-源码解析-QVideoWidget播放手机视频旋转问题

## 问题描述与复现

使用手机拍摄的视频是竖屏的，上传后，使用QVideoWidget播放，变成横屏。

总结，可以让播放器能够旋转，即可解决此问题。

## 测试准备

视频测试地址： http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4 

## 问题分析

QWidget的旋转方法两种，

- 重载::paintEvent(QPaintEvent **e*)函数，使用QTransform实现旋转。
- 借助QGraphicsView的rotate()函数实现旋转

QVideoWidget不支持旋转。查看setVideoOutput函数进一步获取支持。

```c++
void QMediaPlayer::setVideoOutput(QVideoWidget *output)
Attach a QVideoWidget video output to the media player.
If the media player has already video output attached, it will be replaced with a new one.
void QMediaPlayer::setVideoOutput(QGraphicsVideoItem *output)
Attach a QGraphicsVideoItem video output to the media player.
If the media player has already video output attached, it will be replaced with a new one.
void QMediaPlayer::setVideoOutput(QAbstractVideoSurface *surface)
Sets a video surface as the video output of a media player.
If a video output has already been set on the media player the new surface will replace it.
```



## 问题解决方法

当尝试QWidget的旋转方法后，发现并不生效。他们是针对QWidget本身的绘制。

setVideoOutput，采用专用的类后，可以实现旋转。关键代码如下：

```c++
//Step 1   
m_videoItem = new QGraphicsVideoItem;
QGraphicsScene *scene = new QGraphicsScene(this);
QGraphicsView *graphicsView = new QGraphicsView(scene);

scene->addItem(m_videoItem);

//Step 2
m_mediaPlayer->setVideoOutput(m_videoItem);

//Step 3	按照中心点旋转
qreal x = m_videoItem->boundingRect().width() / 2.0;
qreal y = m_videoItem->boundingRect().height() / 2.0;
m_videoItem->setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));

```



## 实践问题1：

启动后报错，DirectShowPlayerService::doRender: Unresolved error code 0x80040266 (IDispatch error #102)

安装 **LAV Filters**，<a href="http://files.1f0.de/lavf/LAVFilters-0.65.exe" rel="nofollow">LAV Filters download</a>

