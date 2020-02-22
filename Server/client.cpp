#include <iostream>
#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024
#define FILENAME_SIZE 200
#define BUFFER_SIZE 1024
#define IPLEN 100

using std::cout;
using std::cin;
using std::endl;

struct FileInfo {
	char name[200];
	unsigned long fileSize;  
};

int main() {
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

	

	struct sockaddr_in servaddr, cliaddr;
	int sockfd, addr_len, ret;
	struct FileInfo file;
	addr_len = sizeof(sockaddr);



	const char * filePath = "D:/Client.c";
	char drive[20];
	char dir[100];
	char fname[100];
	char ext[10];
	_splitpath_s(filePath, drive, dir, fname, ext);
	cout << drive << "  " << dir << "  " << fname << "  " << ext << endl;
	cout << strlen(fname) << endl;
	char *fileName = new char[strlen(fname) + strlen(ext) + 1];
	cout << strlen(fname) << endl;
	cout << strlen(ext) << endl;
	strcpy_s(fileName, strlen(fname)+1, fname);
	//strcat_s(fileName, strlen(fileName) + strlen(ext) + 1, ext);
	strcpy_s(fileName + strlen(fname), strlen(ext) + 1, ext);
	strcpy_s(file.name, strlen(fileName)+1, fileName);
	delete fileName;

	struct stat f_stat;
	//stat(filePath, &f_stat);
	file.fileSize = f_stat.st_size;
	
	memset(&servaddr, 0, addr_len);
	servaddr.sin_family = AF_INET;
	//inet_pton(AF_INET, "192.168.28.146", &servaddr.sin_addr.s_addr);
	//inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);
	servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
	
	
	servaddr.sin_port = htons(SERVER_PORT);
	cout << "helloaaa" << endl;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	ret =connect(sockfd, (struct sockaddr *)&servaddr, addr_len);

	
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	send(sockfd, (char*)&file, sizeof(file), 0);
	FILE *fp;
	fopen_s(&fp, filePath, "rb");
	int fileBlockLength = 0;
	while ((fileBlockLength = fread(buffer, sizeof(char), BUFFER_SIZE, fp))>0) {
		cout << buffer << endl;
		cout << fileBlockLength << endl;
		if(send(sockfd, buffer, strlen(buffer), 0)<0)
			break;

	}
	fclose(fp);
	cout << "file trans " << endl;
	


	closesocket(sockfd);
	WSACleanup();
	
	cin.get();
	return 0;
}