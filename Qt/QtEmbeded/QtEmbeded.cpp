#include "QtEmbeded.h"

#include <QWindow>

#include<Windows.h>


QtEmbeded::QtEmbeded(QWidget *parent)
    : QWidget(parent),calcID(-1)
{
    ui.setupUi(this);

	//calcID = (WId)FindWindow(L"Windows.UI.Core.CoreWindow", NULL);
	//calcID = (WId)FindWindow(NULL, L"截图和草图");
	//calcID = (WId)FindWindow(L"Chrome_WidgetWin_1", NULL);//浏览器
	calcID = (WId)FindWindow(L"Notepad++", NULL);//
}

void QtEmbeded::initQt()
{
	if (NULL == calcID)
	{
		return;
	}
	windowWnd = QWindow::fromWinId(calcID);
	//windowWnd->setWidth(this->width());
	//windowWnd->setHeight(this->height());无效
	
	childWdg = QWidget::createWindowContainer(windowWnd,this);
	childWdg->setMinimumSize(QSize(500, 600));//必须设置大小，否则可能因为嵌入后窗口太小看不到效果
}

void QtEmbeded::initWin()
{
	HWND hWnd = (HWND)this->winId();
	HWND calcHwnd = (HWND)calcID;
	if (NULL == calcID)
	{
		return;
	}
	// Change the parent so the calc window belongs to our apps main window 
	SetParent(calcHwnd, hWnd);

	// Update the style so the calc window is embedded in our main window
	SetWindowLong(calcHwnd, GWL_STYLE, GetWindowLong(calcHwnd, GWL_STYLE) | WS_CHILD);

	// We need to update the position as well since changing the parent does not
	// adjust it automatically.
	SetWindowPos(calcHwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
