#include "QtEmbeded.h"

#include <QWindow>

#include<Windows.h>


QtEmbeded::QtEmbeded(QWidget *parent)
    : QWidget(parent),calcID(-1)
{
    ui.setupUi(this);

	//calcID = (WId)FindWindow(L"Windows.UI.Core.CoreWindow", NULL);
	//calcID = (WId)FindWindow(NULL, L"��ͼ�Ͳ�ͼ");
	//calcID = (WId)FindWindow(L"Chrome_WidgetWin_1", NULL);//�����
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
	//windowWnd->setHeight(this->height());��Ч
	
	childWdg = QWidget::createWindowContainer(windowWnd,this);
	childWdg->setMinimumSize(QSize(500, 600));//�������ô�С�����������ΪǶ��󴰿�̫С������Ч��
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
