/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* ��ӭ����ָ�� xiaoxiaomiantianwo@163.com
******************************************************************************************/
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/event_compat.h>
#include <event2/event_struct.h>

#include "RemotingServer.h"

namespace lxmq{

struct event_base *base;

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

		//���ӽ���(=0)�����Ӵ���<0),���¼�ɾ�����ͷ��ڴ�ռ�
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

}

void onAccept(evutil_socket_t iSvrFd, short iEvent, void* arg)
{
	evutil_socket_t iCliFd;
	struct sockaddr_in sCliAddr;

	socklen_t iSinSize = sizeof(sCliAddr);
	iCliFd = accept(iSvrFd, (sockaddr*)&sCliAddr, &iSinSize);

	//����ע��Ϊ���¼�
	struct event *pEvRead = new event;
	event_set(pEvRead, iCliFd, EV_READ | EV_PERSIST, onRead, pEvRead);
	event_base_set(base, pEvRead);
	event_add(pEvRead, NULL);
}
RemotingServer::RemotingServer()
{

}

RemotingServer::~RemotingServer()
{

}

void RemotingServer::init()
{
	evutil_socket_t iSvrFd;
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

	//����tcp socket��������9876�˿�
	iSvrFd = socket(AF_INET, SOCK_STREAM, 0);
	bind(iSvrFd, (struct sockaddr*)&sSvrAddr, sizeof(sSvrAddr));
	listen(iSvrFd, 10);

	/* Initalize the event library */
	base = (struct event_base*)event_init();

	struct event evListen;

	//�����¼�
	event_set(&evListen, iSvrFd, EV_READ | EV_PERSIST, onAccept, NULL);

	//����Ϊbase�¼�
	event_base_set(base, &evListen);

	//����¼�
	event_add(&evListen, NULL);

	//�¼�ѭ��
	event_base_dispatch(base);
}


}//    end of lxmq
