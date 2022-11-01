// Web01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#define  WIN32_LEAN_AND_MEAN
#include <iostream>
#include "windows.h"
#include "winsock2.h"
#include "process.h"
#pragma comment(lib,"ws2_32.lib")
#define  PORT 5698
#define  IP "127.0.0.1" 
//客户端信息结构体
typedef struct clientInformationn
{
	SOCKET socket;
	SOCKADDR_IN sin;
	//初始化
	clientInformationn()
	{
		socket = INVALID_SOCKET;
		ZeroMemory(&sin,sizeof(SOCKADDR_IN));
	
	}
}clientInformationn;
unsigned __stdcall MyBeginthreadex(void* lpParam) {
	clientInformationn *client = (clientInformationn*)lpParam;
	if (client==NULL)
	{
		return -1;
	}
	while (1)
	{
		char recvbuf[1024];
		int recvbuflen = 512;
		char recvbufSend[512]="我是服务器.我已经接收到数据并向你返回数据";
		int recvbuflenSend = 512;
		ZeroMemory(&recvbuf, recvbuflen);//清空字符缓冲区
		int ret=recv(client->socket, recvbuf, 1024 -sizeof(TCHAR), 0);
		switch (ret)
		{
		case 0:
			printf("客户端:%s :%d 断开链接 error = %d\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port), WSAGetLastError());
			break;
			//return -2; //不用return 因为还需要继续等待链接
		case SOCKET_ERROR:
			printf("客户端:%s :%d接收数据失败 error = %d\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port), WSAGetLastError());
			break;
			//return -3;
		default:
			printf("客户端:%s :%d接收数据成功:  %s\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port), recvbuf);
		}
		
		int retSend=send(client->socket, recvbufSend, strlen(recvbufSend) + 1, 0);
		switch (retSend)
		{
		case SOCKET_ERROR:
			printf("服务器端发送数据失败 error = %d\n", WSAGetLastError());
			break;
		}
	}
	if (client)
	{
		delete client;
		client = NULL;
	}
	return -5;
};
int main()
{
	//1.初始化网络环境
	WORD version=MAKEWORD(2, 2);
	WSADATA data = { 0 };
	int isTrue=WSAStartup(version, &data);//通过进程启动 Winsock DLL 的使用。
	if (isTrue!=0)
	{
		printf("WSAStartup启动失败 error = %d\n", WSAGetLastError());
		return isTrue;
	}
	printf("WSAStartup启动成功\n");
	//2.创建socket
	SOCKET  sock =socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket 创建失败 error = %d\n", WSAGetLastError());
		return WSAGetLastError();
	}
	printf("创建socket成功\n");
	//3.绑定IP 端口
	SOCKADDR_IN sinServer; 
	sinServer.sin_family = AF_INET;
	sinServer.sin_addr.s_addr = inet_addr(IP);
	sinServer.sin_port = htons(PORT); //大端
	if (bind(sock, (SOCKADDR*)&sinServer, sizeof(SOCKADDR_IN))== SOCKET_ERROR)
	{
		printf("socket 绑定失败 error = %d\n", WSAGetLastError());

		return -1;
	}
	printf("绑定IP 端口成功\n");
	//4.监听端口
	if (listen(sock, SOMAXCONN)==SOCKET_ERROR)
	{
		printf("监听设置失败\n");
	}
	printf("监听设置成功\n");
	//5.等待客户端链接
	SOCKADDR_IN sinClient;
	int size = sizeof(SOCKADDR);
	while (1)
	{
		//堵塞函数
		SOCKET sockClient=accept(sock, (SOCKADDR*)&sinClient, &size);
		if (sockClient == INVALID_SOCKET)
		{
			printf("等待客户端链接失败 error = %d\n", WSAGetLastError());
			return -1;
		}
		printf("客户端链接成功 IP:%s 端口(原始):%d 端口(小端):%d\n", inet_ntoa(sinClient.sin_addr), sinClient.sin_port, ntohs(sinClient.sin_port));
		unsigned int adress = 0;
		clientInformationn *client=new clientInformationn();
		client->socket = sockClient;
		client->sin = sinClient;
		_beginthreadex(NULL, NULL, MyBeginthreadex, client, 0, &adress);
	}


	shutdown(sock, 2);
	closesocket(sock);
	WSACleanup();

}
