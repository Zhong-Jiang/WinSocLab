// EchoTCPServer.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//��������Ϸ��Լ��
	if (argc != 1)
	{
		printf("usage: EchoTCPServer");
		return -1;
	}
	
	//Windows Sockets Dll��ʼ��
	frame.start_up();

	//�����������˵���ʽ�׽��ֲ���ָ���˿ں��ϼ���
	ListenSocket = frame.tcp_server( NULL, ECHOPORT );
	if ( ListenSocket == -1 )
		return -1;
    printf("�������Ѿ�׼���û������\n");
	for ( ; ; ) {
		ConnectSocket = accept( ListenSocket, NULL, NULL );
		if( ConnectSocket != INVALID_SOCKET ){
			//�������ӳɹ�
			printf("\r\n�������ӳɹ�\n\n");
			//����
			iResult = tcp_server_fun_echo( ConnectSocket );
			//��������رյ�ǰ�����׽��֣��������������ͻ��˵�����
			if(iResult == -1)
				printf("��ǰ�����ѹرջ����!\n");
		}
		else{
			printf("accept �������ô��󣬴���ţ� %d\n", WSAGetLastError());
            frame.quit( ListenSocket );
            return -1;
        }

		//�ر������׽���
		if ( closesocket( ConnectSocket ) == SOCKET_ERROR)
	        printf("closesocket �������ô��󣬴���ţ�%d\n", WSAGetLastError());;
	}
	frame.quit( ListenSocket );
    return 0;
}
/********************************************************
��������tcp_server_fun
���������SOCKET s:�������������׽���
���������0���ɹ���-1��ʧ��
���ܣ�tcp_client����ͻ��˵ľ��幦�ܺ���
********************************************************/
int tcp_server_fun_echo( SOCKET s )
{
	int iResult = 0;
	char    recvline[MAXLINE];
	do {
		memset( recvline, 0, MAXLINE );
		//��������
 	    iResult = recv( s, recvline, MAXLINE, 0);
		if (iResult > 0){
			printf("�������˽��յ�����%s\n", recvline);
			
			//���䷢�����յ�������
			iResult = send( s,recvline,iResult, 0 );
			if(iResult == SOCKET_ERROR)
			{
				printf("send �������ô��󣬴����: %ld\n", WSAGetLastError());
				iResult = -1;
			}
			else
			    printf("�������˷�������%s\n", recvline);
		}
		else{
			if (iResult == 0)
 	            printf("�Է����ӹرգ��˳�\n");
		    else{
 	            printf("recv �������ô��󣬴����: %d\n", WSAGetLastError());
			    iResult = -1;
			}
			break;
		}
 	} while (iResult > 0);

	return iResult;
}

