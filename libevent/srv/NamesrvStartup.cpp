#include <stdio.h>    
#include <string.h>    
#include <iostream>  
using namespace std;

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
//事件base

#include<event.h>  

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
		cout << "client close" << endl;

		//连接结束(=0)或连接错�?<0),将事件删除并释放内存空间
		struct event *pEvRead = (struct event*)arg;
		event_del(pEvRead);
		delete pEvRead;

		evutil_closesocket(iCliFd);

		return;
	}

	buf[iLen] = 0;
	cout << "Client Info:" << buf << endl;

	std::string MESSAGE = "welcome to server...";
	struct event* write_ev = new event;
	event_set(write_ev, iCliFd, EV_WRITE, onWrite, (void*)MESSAGE.data());
	event_base_set(base, write_ev);
	event_add(write_ev, NULL);

}

void onAccept(evutil_socket_t iSvrFd, short iEvent, void* arg)
{
	int iCliFd;
	struct sockaddr_in sCliAddr;

	socklen_t iSinSize = sizeof(sCliAddr);
	iCliFd = accept(iSvrFd, (sockaddr*)&sCliAddr, &iSinSize);

	//连接注册为新事件�?
	struct event *pEvRead = new event;
	event_set(pEvRead, iCliFd, EV_READ | EV_PERSIST, onRead, pEvRead);
	event_base_set(base, pEvRead);
	event_add(pEvRead, NULL);
}

int main()
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
		return 1;
	}

	InetPton(AF_INET, TEXT("127.0.0.1"), &sSvrAddr.sin_addr.s_addr);
#else
	sSvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif

	sSvrAddr.sin_port = htons(9876);

	//创建tcp socket，监听本9876端口
	iSvrFd = socket(AF_INET, SOCK_STREAM, 0);
	bind(iSvrFd, (struct sockaddr*)&sSvrAddr, sizeof(sSvrAddr));
	listen(iSvrFd, 10);

	/* Initalize the event library */
	base = (struct event_base*)event_init();

	struct event evListen;

	//设置事件
	event_set(&evListen, iSvrFd, EV_READ | EV_PERSIST, onAccept, NULL);

	//设置为base事件
	event_base_set(base, &evListen);

	//添加事件
	event_add(&evListen, NULL);

	//事件循环
	event_base_dispatch(base);
}