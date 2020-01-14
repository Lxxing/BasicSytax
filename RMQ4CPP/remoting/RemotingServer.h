/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __REMOTING_SERVER__H_
#define __REMOTING_SERVER__H_


#ifdef _WIN32
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")// win Need to link with Ws2_32.lib
#endif

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>

#include "RemotingUtil.h"
using namespace std;

namespace lxmq{

class RemotingServer
{
public:

	RemotingServer();
	~RemotingServer();

	void init();
	
	void* invokeSync(const string& addr, int timeoutMillis = 3000);
	
	bool invokeAsync(const string& addr,
					 int timeoutMilliseconds,
					 int maxRetrySendTimes = 1,
					 int retrySendTimes = 1);
	
	void invokeOneway(const string& addr);

private:
	
	string addr;
};
	
	
	
}//    end of lxmq
	
#endif //__REMOTING_SERVER__H_

