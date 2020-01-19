/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#include <DefaultRequestProcessor.h>
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
	this->remotingServer = std::make_shared<RemotingServer>();

	this->remotingServer->init();
	this->remotingServer->start();

	this->registerProcessor();

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
	this->remotingServer->registerDefaultProcessor(std::make_shared<DefaultRequestProcessor>(this));
}


const std::shared_ptr<RouteInfoManager> & NamesrvController::getRouteInfoManager()
{

	return this->routeInfoManager;
}




}//    end of lxmq

