#include "QTableTest.h"

#include <QTableWidgetItem> 
#include <QTreeWidgetItem>
#include <QCheckBox>
#include <QDebug>

QTableTest::QTableTest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	mCheckALL = new QCheckBox();
}

void QTableTest::init()
{

	ui.tableWidget->clear();

	

	QStringList strList;
	strList << "A" << "B" << "C";
	ui.tableWidget->setRowCount(10);
	ui.tableWidget->setColumnCount(strList.size());
	ui.tableWidget->setHorizontalHeaderLabels(strList);


	//ui.tableWidget->setCellWidget(0,1,mCheckALL);
	for (int i = 0; i < 10; i++)
	{
		for (int row = 0; row < strList.size(); row++)
		{
			QTableWidgetItem * item = new QTableWidgetItem();

			if (row != 0)
			{
				item->setText(QString::number(i));
				if (1 == row)
				{
					item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
			}
			else
			{
				item->setText(QString("index") + QString::number(i));
				item->setCheckState(Qt::Unchecked);
			}

			ui.tableWidget->setItem(i, row, item);
		}
		
	}
}

void QTableTest::on_btnALL_clicked()
{
	int row_count = ui.tableWidget->rowCount();  
	for (int i = 0; i < row_count; i++)
	{
		QTableWidgetItem *widgetSex = ui.tableWidget->item(i, 0);
		if (widgetSex->checkState() != Qt::Checked)    //如果checkbox项被选中
		{
			QString sTablename = ui.tableWidget->item(i, 0)->text();    //获取第一列的值

			qDebug() << sTablename;
			widgetSex->setCheckState(Qt::Checked);
		}
	}
}
