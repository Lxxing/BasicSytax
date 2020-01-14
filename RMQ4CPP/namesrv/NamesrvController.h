/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __NAMESRV_CONSTROLLER__H_
#define __NAMESRV_CONSTROLLER__H_

#include <iostream>  
using namespace std;

#include <RemotingServer.h>

namespace lxmq{

class NamesrvController{

public:
	NamesrvController();
	~NamesrvController();

	bool initialize();

	void start();

	void shutdown();

	void registerProcessor();

private:
	RemotingServer remotesrv;


};//end of NamesrvController

}//    end of lxmq


#endif //__NAMESRV_CONSTROLLER__H_

