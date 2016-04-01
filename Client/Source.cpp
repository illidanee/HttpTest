#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment (lib, "Ws2_32.lib")

extern void TestGetHttpRequest(SOCKET server);
extern void TestPostHttpRequest(SOCKET server);

int main(int argc, char** argv)
{
	//启动网络环境
	WSAData wsaData;
	WORD wsaVersion = MAKEWORD(2, 2);
	int res = WSAStartup(wsaVersion, &wsaData);
	if (res != 0)
		return -1;


	//创建套接字
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		WSACleanup();
		return -2;
	}


	//服务地址和端口
	const char* pServerUrl = "www.baidu.com";
	HOSTENT* hostent = gethostbyname(pServerUrl);
	if (hostent == 0)
	{
		closesocket(server);
		WSACleanup();
		return -3;
	}
	unsigned short port = 80;

	sockaddr_in serverAdd;
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_addr = *(IN_ADDR*)hostent->h_addr_list[0];
	serverAdd.sin_port = htons(port);


	//连接服务器
	res = connect(server, (sockaddr*)&serverAdd, sizeof(sockaddr));
	if (res != 0)
	{
		closesocket(server);
		WSACleanup();
		return -4;
	}


	//发送HTTP报文
	TestGetHttpRequest(server);
	TestPostHttpRequest(server);


	//退出网络环境
	closesocket(server);
	WSACleanup();

	return 0;
}


void TestGetHttpRequest(SOCKET server)
{
	printf("================================================================================\r\n");

	char* pRequestLine = 
		"GET http://www.baidu.com/ HTTP/1.1\r\n";
	char* pRequestHead = 
		"Host: www.baidu.com:80\r\n";
	char* pRequestBlank =
		"\r\n";
	char* pRequestContext = 
		"";

	std::string info = pRequestLine;
	info += pRequestHead;
	info += pRequestBlank;
	info += pRequestContext;


	int numS = send(server,  info.c_str(), info.length(), 0);

	char rev[1024] = {};
	int numR = recv(server, rev, 1024, 0);
	printf(rev);
}

void TestPostHttpRequest(SOCKET server)
{
	printf("================================================================================\r\n");

	char* pRequestLine = 
		"POST http://www.baidu.com HTTP/1.1\r\n";
	char* pRequestHead = 
		"Host: www.baidu.com\r\n";
	char* pRequestBlank =
		"\r\n";
	char* pRequestContext = 
		" ";

	std::string info = pRequestLine;
	info += pRequestHead;
	info += pRequestBlank;
	info += pRequestContext;


	int numS = send(server,  info.c_str(), info.length(), 0);

	char rev[10240] = {};
	int numR = recv(server, rev, 10240, 0);
	printf(rev);
}