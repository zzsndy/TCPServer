#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>

#define SERVER_PORT 8888
#define LISTEN_QUEUE 128
#define FILENAME_SIZE 200
#define BUFFER_SIZE 1024
#define IPLEN 100

using std::cin;
using std::cout;
using std::endl;

struct FileInfo
{
	char name[200];
	unsigned long fileSize;
};

class TransServer
{
private:
	int listenfd, clifd;				  // socket 监听 、客户端
	struct sockaddr_in servaddr, cliaddr; // server和 client sockadd
	struct FileInfo file;				  // file info
	int addrLen, recvLen;				  // 地址长度和接收的长度
	int fileInfoLen;					  // FileInfo结构长度
	char buffer[BUFFER_SIZE];
	char ip[IPLEN];

public:
	TransServer();
	~TransServer();
	void start();	//开始监听连接
	int transFile(); //传输文件
};

TransServer::TransServer()
{

	WORD verision = MAKEWORD(2, 2);
	WSADATA lpData;
	int intEr = WSAStartup(verision, &lpData); //指定winsock版本并初始化
	if (intEr != 0)
	{
		cout << "winsock init failed!" << endl;
	}
	else
		cout << "winsock init success!" << endl;

	addrLen = sizeof(sockaddr);
	fileInfoLen = sizeof(FileInfo);
	memset(&file, 0, fileInfoLen);
	memset(&servaddr, 0, addrLen);
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	bind(listenfd, (struct sockaddr *)&servaddr, addrLen);
	listen(listenfd, LISTEN_QUEUE);

	cout << "Accepting connections ....\n";
}

TransServer::~TransServer()
{

	closesocket(listenfd);
	WSACleanup();
}

int TransServer::transFile()
{

	cout << "transFile" << endl;
	return 0;
}

void TransServer::start()
{
	while (true)
	{
		memset(&cliaddr, 0, addrLen);
		clifd = accept(listenfd, (struct sockaddr *)&cliaddr, &addrLen);
		if (clifd == -1)
		{
			perror("clifd error");
			cout << "hello world" << endl;
			continue;
		}
		cout << "hello: " << inet_ntoa(cliaddr.sin_addr) << "  connect success" << endl;

		memset(&buffer, 0, BUFFER_SIZE);
		recvLen = recv(clifd, buffer, BUFFER_SIZE, 0);
		memset(&file, 0, fileInfoLen);
		memcpy(&file, buffer, fileInfoLen);
		cout << file.name << endl;
		cout << file.fileSize << endl;

		if (recvLen < 0)
		{
			cout << "Recv data failed" << endl;
			continue;
		}

		char fileName[FILENAME_SIZE];
		memset(fileName, 0, FILENAME_SIZE);
		strncpy_s(fileName, buffer, strlen(buffer));
		FILE *fp;
		fopen_s(&fp, fileName, "wb");
		if (fp == NULL)
		{
			cout << "open file failed" << endl;
		}
		memset(&buffer, 0, BUFFER_SIZE);

		unsigned long recvSize = 0;
		while (true)
		{
			recvLen = recv(clifd, buffer, BUFFER_SIZE, 0);
			cout << buffer << endl;
			cout << recvLen << endl;
			cout << strlen(buffer) << endl;

			recvSize += strlen(buffer);

			fwrite(buffer, sizeof(char), BUFFER_SIZE, fp);
			if (recvSize >= file.fileSize)
				break;
		}

		fclose(fp);
		cout << file.name << "write wancheng" << endl;
	}

	return;
}



int main()
{
	TransServer server = TransServer();
	server.start();

	cin.get();
	return 0;
}