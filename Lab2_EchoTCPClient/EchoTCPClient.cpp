// EchoTCPClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "SocketFrame.h"
int tcp_client_fun_echo(FILE *fp,SOCKET s);
#define ECHOPORT "7210"

/********************************************************
函数名：tcp_client_fun_echo
输入参数：FILE *fp:指向FILE类型的对象
		  SOCKET s:客户端的连接套接字
输出参数：0：成功，-1：失败
功能：tcp_client回射客户端的具体功能函数
*********************************************************/
int tcp_client_fun_echo(FILE* fp, SOCKET s)
{
	int iResult;
	char sendline[MAXLINE], recvline[MAXLINE];
	memset(sendline, 0, MAXLINE);
	memset(recvline, 0, MAXLINE);

	//循环发送用户的输入数据，并接收服务器返回的应答，直到用户输入"Q"结束
	while (fgets(sendline, MAXLINE, fp) != NULL)
	{
		if (*sendline == 'Q') {
			printf("input end!\n");
			// 数据发送结束，调用shutdown()函数声明不再发送数据，此时客户端仍可以接收数据
			iResult = shutdown(s, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				printf("shutdown failed with error: %d\n", WSAGetLastError());
			}
			return 0;
		}
		iResult = send(s, sendline, strlen(sendline), 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send 函数调用错误，错误号： %ld\n", WSAGetLastError());
			return -1;
		}
		printf("\r\n客户端发送数据：%s\r\n", sendline);

		memset(recvline, 0, MAXLINE);
		iResult = recv(s, recvline, MAXLINE, 0);
		if (iResult > 0)
			printf("客户端接收到数据：%s \r\n", recvline);
		else {
			if (iResult == 0)
				printf("服务器终止!\n");
			else
				printf("recv 函数调用错误，错误号: %d\n", WSAGetLastError());
			break;
		}
		memset(sendline, 0, MAXLINE);
	}
	return iResult;
}
int main(int argc, char *argv[])
{
	CSocketFrame frame;
	int iResult;
	SOCKET ClientSocket;


	//输入参数合法性检查
	if (argc != 2)
	{
		printf("usage: EchoTCPClient <IPaddress>");
		return -1;
	}

	//Windows Sockets Dll初始化
	frame.start_up();
	
	//创建客户端的流式套接字，并与服务器建立连接
    printf("连接建立成功，请输入回射字符串。。。\n");
	ClientSocket = frame.tcp_client( ( char *)argv[1], ECHOPORT );
	if ( ClientSocket == -1 )
		return -1;

	//开始回射请求的发送与接收
	iResult = tcp_client_fun_echo(stdin,ClientSocket);

	frame.quit( ClientSocket );
    return iResult; 
}

