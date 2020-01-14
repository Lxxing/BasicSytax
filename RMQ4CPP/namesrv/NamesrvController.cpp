/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#include "NamesrvController.h"

namespace lxmq{

NamesrvController::NamesrvController()
{

}

NamesrvController::~NamesrvController()
{

}

bool NamesrvController::initialize()
{
	this->remotesrv.init();
	this->remotesrv.start();

	return true;
}

void NamesrvController::start()
{

}

void NamesrvController::shutdown()
{

}

void NamesrvController::registerProcessor()
{

}


	
}//    end of lxmq

