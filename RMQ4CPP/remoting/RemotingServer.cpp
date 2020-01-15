/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/
#include <boost/cstdint.hpp>

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/event_compat.h>
#include <event2/event_struct.h>
#include <event2/listener.h> 


#include "RemotingServer.h"

namespace lxmq{

RemotingServer::RemotingServer()
{
	SocketInit();

	/* Initalize the event library */
	m_eventBase = event_base_new();

}

RemotingServer::~RemotingServer()
{
	evconnlistener_free(m_listener);

	event_base_free(m_eventBase);
}

void RemotingServer::init()
{

	bind();

}

void RemotingServer::SocketInit()
{
#ifdef _WIN32
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

#endif

}

void RemotingServer::start()
{

    m_loopThread = new std::thread(&RemotingServer::runLoop, this);
}

void RemotingServer::bind()
{
	struct sockaddr_in sSvrAddr;
	
	memset(&sSvrAddr, 0, sizeof(sSvrAddr));
	sSvrAddr.sin_family = AF_INET;
#ifdef _WIN32
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}
	
	InetPton(AF_INET, TEXT("127.0.0.1"), &sSvrAddr.sin_addr.s_addr);
#else
	sSvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif
	
	sSvrAddr.sin_port = htons(9876);

	m_listener = evconnlistener_new_bind(m_eventBase,listener_callback,this,
		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE, 
		10, (struct sockaddr*)&sSvrAddr, sizeof(struct sockaddr_in));

}

void RemotingServer::stop()
{

}

void RemotingServer::runLoop()
{
	_is_running = true;
	
	 while (_is_running) 
	 {
	   int ret;
	
	   ret = event_base_dispatch(m_eventBase);
	   if (ret == 1) {
		 // no event
		 std::this_thread::sleep_for(std::chrono::milliseconds(1));
	   }
	 }

}

	

void RemotingServer::listener_callback(evconnlistener *listener,
		evutil_socket_t fd,struct sockaddr *sock, int socklen, void *arg)
{
	RemotingServer * rs = (RemotingServer*)arg;
	event_base *base = rs->GetEventBase();

	//连接注册为新事件
	bufferevent *bEvent = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bEvent, read_callback, NULL, NULL, rs);
	bufferevent_enable(bEvent, EV_READ | EV_PERSIST); 
}

void RemotingServer::read_callback(struct bufferevent* bev, void* ctx)
{
	const RemotingServer *rs = (const RemotingServer *)ctx;
	struct evbuffer* input = bufferevent_get_input(bev);
	while (1)
	{
		struct evbuffer_iovec v[4];
		int n = evbuffer_peek(input, 4, NULL, v, sizeof(v) / sizeof(v[0]));

		char hdr[4];
		char* p = hdr;
		size_t needed = 4;//the head lease length

		for (int idx = 0; idx < n; idx++) {
			if (needed > 0) {
				size_t tmp = needed < v[idx].iov_len ? needed : v[idx].iov_len;
				memcpy(p, v[idx].iov_base, tmp);
				p += tmp;
				needed -= tmp;
			}
			else {
				break;
			}
		}//end of for

		if (needed > 0)
		{
			return;//read next.
		}
		boost::uint32_t totalLenOfOneMsg = *(boost::uint32_t*)hdr;  // first 4 bytes, which indicates 1st part of protocol
		boost::uint32_t msgLen = ntohl(totalLenOfOneMsg);//Network to Host Long
		size_t recvLen = evbuffer_get_length(input);
		if (recvLen >= msgLen + 4) {
			printf("had received all data. msgLen:%d, from:%d, recvLen:%d", msgLen, (int)bufferevent_getfd(bev), recvLen);
		}
		else {
			printf("didn't received whole. msgLen:%d, from:%d, recvLen:%d", msgLen, (int)bufferevent_getfd(bev), recvLen);
			return;  // consider large data which was not received completely by now
		}

		if (msgLen > 0) {
      		MQBuffer msg(msgLen);

			bufferevent_read(bev,hdr,4);// skip length field
			bufferevent_read(bev,(void*)msg.GetData(),msgLen);
      		processMessageReceived(msg,rs);
    	}
	}

}

void RemotingServer::processMessageReceived(const MQBuffer & msg,const RemotingServer *rs)
{
	rs->defaultRequestProcessor->processRequest(msg);
}

void RemotingServer::socket_error_cb(bufferevent *bev, short events, void *arg) 
{
	if (events & BEV_EVENT_EOF)
	    printf ("connection closed\n");
	else if (events & BEV_EVENT_ERROR)
		printf ("BEV_EVENT_ERROR \n");

}

event_base * RemotingServer::GetEventBase() const
{
	return this->m_eventBase;
}

void RemotingServer::registerDefaultProcessor(const std::shared_ptr<RequestProcessor> processor)
{

	this->defaultRequestProcessor = processor;


}




}//    end of lxmq
