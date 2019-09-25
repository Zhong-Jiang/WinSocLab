// EchoTCPServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "SocketFrame.h"
#include "winsock2.h"
#define ECHOPORT "7210"
int tcp_server_fun_echo( SOCKET s );
int main(int argc, char* argv[])
{
	CSocketFrame frame;
	int iResult = 0;
	SOCKET ListenSocket, ConnectSocket;
	//输入参数合法性检查
	if (argc != 1)
	{
		printf("usage: EchoTCPServer");
		return -1;
	}
	
	//Windows Sockets Dll初始化
	frame.start_up();

	//创建服务器端的流式套接字并在指定端口号上监听
	ListenSocket = frame.tcp_server( NULL, ECHOPORT );
	if ( ListenSocket == -1 )
		return -1;
    printf("服务器已经准备好回射服务\n");
	for ( ; ; ) {
		ConnectSocket = accept( ListenSocket, NULL, NULL );
		if( ConnectSocket != INVALID_SOCKET ){
			//建立连接成功
			printf("\r\n建立连接成功\n\n");
			//回射
			iResult = tcp_server_fun_echo( ConnectSocket );
			//如果出错，关闭当前连接套接字，继续接收其它客户端的请求
			if(iResult == -1)
				printf("当前连接已关闭或出错!\n");
		}
		else{
			printf("accept 函数调用错误，错误号： %d\n", WSAGetLastError());
            frame.quit( ListenSocket );
            return -1;
        }

		//关闭连接套接字
		if ( closesocket( ConnectSocket ) == SOCKET_ERROR)
	        printf("closesocket 函数调用错误，错误号：%d\n", WSAGetLastError());;
	}
	frame.quit( ListenSocket );
    return 0;
}
/********************************************************
函数名：tcp_server_fun
输入参数：SOCKET s:服务器的连接套接字
输出参数：0：成功，-1：失败
功能：tcp_client回射客户端的具体功能函数
********************************************************/
int tcp_server_fun_echo( SOCKET s )
{
	int iResult = 0;
	char    recvline[MAXLINE];
	do {
		memset( recvline, 0, MAXLINE );
		//接收数据
 	    iResult = recv( s, recvline, MAXLINE, 0);
		if (iResult > 0){
			printf("服务器端接收到数据%s\n", recvline);
			
			//回射发送已收到的数据
			iResult = send( s,recvline,iResult, 0 );
			if(iResult == SOCKET_ERROR)
			{
				printf("send 函数调用错误，错误号: %ld\n", WSAGetLastError());
				iResult = -1;
			}
			else
			    printf("服务器端发送数据%s\n", recvline);
		}
		else{
			if (iResult == 0)
 	            printf("对方连接关闭，退出\n");
		    else{
 	            printf("recv 函数调用错误，错误号: %d\n", WSAGetLastError());
			    iResult = -1;
			}
			break;
		}
 	} while (iResult > 0);

	return iResult;
}

