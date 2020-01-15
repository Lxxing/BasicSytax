/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __REMOTING_SERVER__H_
#define __REMOTING_SERVER__H_

#include <memory>
#include <thread>

#ifdef _WIN32
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")// win Need to link with Ws2_32.lib
#endif

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

#include <MQBuffer.h>
#include <MQTypes.h>
#include <RequestProcessor.h>


#include "RemotingUtil.h"
using namespace std;

namespace lxmq{

class RemotingServer
{
public:

	RemotingServer();
	~RemotingServer();

	void init();
	void start();
	void stop();
	void bind();

	
	void* invokeSync(const string& addr, int timeoutMillis = 3000);
	
	bool invokeAsync(const string& addr,
					 int timeoutMilliseconds,
					 int maxRetrySendTimes = 1,
					 int retrySendTimes = 1);
	
	void invokeOneway(const string& addr);
	event_base * GetEventBase() const;
	void registerDefaultProcessor(const           std::shared_ptr<RequestProcessor> processor);

	static void read_callback(struct bufferevent* bev, void* ctx);
	static void listener_callback(evconnlistener *listener,
		evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg);
		
	static void socket_error_cb(bufferevent *bev, short events, void *arg); 

private:
	 void runLoop();
	 void SocketInit();
	 static void processMessageReceived(const MQBuffer & msg,const RemotingServer *rs);

private:
	evutil_socket_t m_fd;

	struct event_base* m_eventBase;
	struct bufferevent* m_bufferEvent;
	evconnlistener *m_listener;
	std::thread* m_loopThread;
	bool _is_running; 
	std::shared_ptr<RequestProcessor> defaultRequestProcessor;
};
	
	
}//    end of lxmq
	
#endif //__REMOTING_SERVER__H_

