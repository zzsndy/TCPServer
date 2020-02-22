#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

// #pragma comment(lib, "ws2_32.lib")


#define SERVER_PORT 8888
#define LISTEN_QUEUE 128
#define FILENAME_SIZE 200
#define BUFFER_SIZE 1024

using std::cin;
using std::cout;
using std::endl;

struct FileInfo {
	char name[200];
	unsigned long fileSize;
};

class transServer {
private:

public:
	transServer();
	~transServer();

};

transServer::transServer() {

}

transServer::~transServer() {

}

int main()
{

	WORD verision = MAKEWORD(2, 2);
	WSADATA lpData;
	int intEr = WSAStartup(verision, &lpData);//指定winsock版本并初始化
	if (intEr != 0)
	{
		cout << "winsock init failed!" << endl;
		return 0;
	}
	else
		cout << "winsock init success!" << endl;

	int listenfd, clifd;
	struct sockaddr_in servaddr, cliaddr;
	struct FileInfo file;
	int addr_len, recv_len;
	int file_info_len;
	char buffer[BUFFER_SIZE];



	addr_len = sizeof(sockaddr);
	file_info_len = sizeof(FileInfo);

	memset(&file, 0, sizeof(FileInfo));
	memset(&servaddr, 0, sizeof(servaddr));

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	bind(listenfd, (struct sockaddr*)&servaddr, addr_len);
	listen(listenfd, LISTEN_QUEUE);
	
	cout << "Accepting connections ....\n";
	while (true) {

		clifd = accept(listenfd, (struct sockaddr*)&cliaddr, &addr_len);
		if (clifd == -1) {
			/*perror("clifd error");
			cout << "hello world" << endl;*/
				continue;
		}
		char ip[100];
		cout << "hello" << endl;
		//cout << inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)) <<"connect success" << endl;
		
		memset(&buffer, 0, BUFFER_SIZE);
		recv_len = recv(clifd, buffer, BUFFER_SIZE, 0);
		memset(&file, 0, file_info_len);
		memcpy(&file, buffer, file_info_len);
		cout << file.name << endl;
		cout << file.fileSize << endl;


		if (recv_len < 0) {
			cout << "Recv data failed" << endl;
			continue;
		}

		char fileName[FILENAME_SIZE];
		memset(fileName, 0, FILENAME_SIZE);
		strncpy_s(fileName, buffer, strlen(buffer));
		FILE * fp;
		fopen_s(&fp, fileName, "wb");
		if (fp == NULL) {
			cout << "open file failed" << endl;
		}
		memset(&buffer, 0, BUFFER_SIZE);

		unsigned long recvSize = 0;
		while (true) {
			recv_len = recv(clifd, buffer, BUFFER_SIZE, 0);
			cout << buffer << endl;
			cout << recv_len << endl;
			cout << strlen(buffer) << endl;

			recvSize += strlen(buffer);
			
			fwrite(buffer, sizeof(char), BUFFER_SIZE, fp);
			if (recvSize >= file.fileSize)
				break;

		}

		fclose(fp);
		cout << file.name << "write wancheng" << endl;
		
		
	}

	closesocket(listenfd);
	WSACleanup();

	cin.get();
	return 0;
}