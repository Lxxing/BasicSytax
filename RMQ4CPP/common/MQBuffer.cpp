/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#include <iostream>
#include <stdio.h>
#include "MQBuffer.h"


namespace lxmq{



MQBuffer::MQBuffer()
{
	this->data = NULL;
	this->length = 0;
}

MQBuffer::MQBuffer(int length)
{
	this->length = length;
	this->data = (char*)malloc(length);
}
MQBuffer::~MQBuffer()
{	
	if (this->data != NULL)
	{
		delete this->data;
	}
}

const char *MQBuffer::GetData() const
{
	return this->data;
}

int MQBuffer::GetSize() const
{
	return this->length;
}


}//    end of lxmq

