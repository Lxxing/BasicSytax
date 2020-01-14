/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#include <boost/shared_ptr.hpp>

#include <stdio.h>   
#include <stdlib.h>
#include <string.h>    
#include <iostream>  
using namespace std;

#include <NamesrvController.h>

using namespace lxmq;

std::shared_ptr<NamesrvController> createNamesrvController()
{
	std::shared_ptr<NamesrvController> controller =
		std::make_shared<NamesrvController>();

	return controller;
}

void start(const std::shared_ptr<NamesrvController> & nsc)
{
	nsc->initialize();
}

void main0()
{
	std::shared_ptr<NamesrvController> controller = createNamesrvController();
    start(controller);
}

int main()
{
	main0();
	
	while (1)
	{
		Sleep(1000);
	}

	return 0;
}
