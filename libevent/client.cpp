#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  
#include <string.h>  
#include <iostream>
using namespace std;
 
#include<event.h>  

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "9876"

int main(int argc, char *argv[])  
{  
		evutil_socket_t sockfd;
        char buffer[1024];  
        struct sockaddr_in server_addr;  
        struct hostent *host;  
		int portnumber = 9876;
		int nbytes = 0;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

#ifdef _WIN32

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Initialize Winsock
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		// Resolve the server address and port
		iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}
#else
        if((host=gethostbyname("127.0.0.1"))==NULL)  
        {  
                fprintf(stderr,"Gethostname error\n");  
                exit(1);  
        }  
#endif
  
        if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)  
        {  
			cout << "Socket Error" << endl;
            exit(1);  
        }  
  
        /* 客户程序填充服务端的资料       */  
        memset(&server_addr,0,sizeof(server_addr));  
        server_addr.sin_family=AF_INET;  
        server_addr.sin_port=htons(portnumber);  
        server_addr.sin_addr=*((struct in_addr *)result->ai_addr);
  
        /* 客户程序发起连接请求         */  
        if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)  
        {  
			cout << "Socket Error" << endl;
			evutil_closesocket(sockfd);
            exit(1);  
        }  
  
        while(true)  
        {  
			char recvbuf[DEFAULT_BUFLEN];
			int nbytes;
			int recvbuflen = DEFAULT_BUFLEN;
            char MESSAGE[]="hello server..\n";  
            //bufferevent_write(buf_ev,MESSAGE,strlen(MESSAGE));  
            //  
            if(-1 == (::send(sockfd,MESSAGE,strlen(MESSAGE),0)))  
            {  
                    printf("the net has a error occured..");  
                    break;  
            }  
 
#ifdef _WIN32
			nbytes = recv(sockfd, recvbuf, recvbuflen, 0);
			if (nbytes > 0)
				 printf("Bytes received: %d\n", nbytes);
			else if (nbytes == 0)
				 printf("Connection closed\n");
			else
				 printf("recv failed with error: %d\n", WSAGetLastError());
#else

           if((nbytes = read(sockfd,buffer,1024))==-1)  
           {  
                   fprintf(stderr,"read error:%s\n",strerror(errno));  
                   exit(1);  
           }  
#endif
			buffer[nbytes]='\0';  
			printf("I have received:%s\n",buffer);  
			memset(buffer,0,1024);  
 
			Sleep(2);  

        }  
		evutil_closesocket(sockfd);
        exit(0);  
}  