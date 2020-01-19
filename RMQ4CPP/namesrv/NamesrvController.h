/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* ��ӭ����ָ�� xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __NAMESRV_CONSTROLLER__H_
#define __NAMESRV_CONSTROLLER__H_

#include <iostream>  
using namespace std;


#include <RemotingServer.h>
#include "RouteInfoManager.h"

namespace lxmq{

class NamesrvController{

public:
	NamesrvController();
	~NamesrvController();

	bool initialize();

	void start();

	void shutdown();

	void registerProcessor();

	const std::shared_ptr<RouteInfoManager> & getRouteInfoManager();
private:
	std::shared_ptr<RemotingServer> remotingServer;
	const std::shared_ptr<RouteInfoManager> routeInfoManager;


};//end of NamesrvController

}//    end of lxmq


#endif //__NAMESRV_CONSTROLLER__H_

