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
/*
void onWrite(evutil_socket_t sock, short iEvent, void* arg)
{
	char* buf = (char*)arg;
	send(sock, buf, strlen(buf), 0);

	//delete buf;
}

void onRead(evutil_socket_t iCliFd, short iEvent, void* arg)
{
	int iLen;
	char buf[1024];

	iLen = recv(iCliFd, buf, sizeof(buf), 0);

	if (iLen <= 0)
	{
		std::cout << "client close" << endl;

		//连接结束(=0)或连接错误<0),将事件删除并释放内存空间
		struct event *pEvRead = (struct event*)arg;
		event_del(pEvRead);
		delete pEvRead;

		evutil_closesocket(iCliFd);

		return;
	}

	buf[iLen] = 0;
	std::cout << "Client Info:" << buf << std::endl;

	std::string MESSAGE = "welcome to server...";
	struct event* write_ev = new event;
	event_set(write_ev, iCliFd, EV_WRITE, onWrite, (void*)MESSAGE.data());
	event_base_set(base, write_ev);
	event_add(write_ev, NULL);

}*/

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

	m_listener = evconnlistener_new_bind(m_eventBase,listener_callback,m_eventBase,
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
	event_base *base = (event_base*)arg;

	//连接注册为新事件
	bufferevent *bEvent = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bEvent, read_callback, NULL, NULL, NULL);
	bufferevent_enable(bEvent, EV_READ | EV_PERSIST); 
}

void RemotingServer::read_callback(struct bufferevent* bev, void* ctx)
{
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
		boost::uint32_t msgLen = ntohl(totalLenOfOneMsg);
		size_t recvLen = evbuffer_get_length(input);
		if (recvLen >= msgLen + 4) {
			printf("had received all data. msgLen:%d, from:%d, recvLen:%d", msgLen, bufferevent_getfd(bev), recvLen);
		}
		else {
			printf("didn't received whole. msgLen:%d, from:%d, recvLen:%d", msgLen, bufferevent_getfd(bev), recvLen);
			return;  // consider large data which was not received completely by now
		}
	}
	char reply[] = "reply form server";
	bufferevent_write(bev, reply, strlen(reply) ); 
}

void RemotingServer::socket_error_cb(bufferevent *bev, short events, void *arg) 
{
	if (events & BEV_EVENT_EOF)
	    printf ("connection closed\n");
	else if (events & BEV_EVENT_ERROR)
		printf ("BEV_EVENT_ERROR \n");

}


}//    end of lxmq
