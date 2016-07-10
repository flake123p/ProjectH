#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_PORT 5555

int main(int argc, char *argv[])
{
	//
	// Create Socket
	//
	int server_socket;
	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(1);
	}
	
	//
	// setsockopt + SO_REUSEPORT : 讓 socket 重開能不被卡住
	//
	int yes = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1)
	{
        perror("setsockopt()");
        exit(1);
    }

	//
	// setsockopt + SO_RCVTIMEO :
	// 為 accept() 與 recv() 設定時間!! 沒錯，兩者都會設
	// 不過之後會再次設定 accept socket, 覆蓋掉 recv() 的 time out 時間
	//
	struct timeval timeout;
	timeout.tv_sec  = 10;
	timeout.tv_usec = 0;
    if(setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1)
	{
        perror("setsockopt()");
        exit(1);
    }
	
	//
	// Bind socket with IP and Port
	//
	struct sockaddr_in server_addr;
	memset((void*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind()");
		exit(1);
	}
	
	//
	// Set a bound TCP socket to listening state
	//
	if(listen(server_socket,5)<0)
	{
		perror("listen()");
		exit(1);
	}
	
	//
	// Accept connection
	//
	int accept_socket;
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof(client_addr);
	char buffer[1024];
	int iDataNum ;
	while(1)
	{
		printf("accept() ... start\n");
		accept_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&client_addr_len);
		if(accept_socket < 0)
		{
			perror("accept()");
			//exit(1);
			break;
		}
		else
		{
			printf("accept() ... done\n");
			printf("Server socket fd = %d\n", server_socket);
			printf("Accept socket fd = %d\n", accept_socket);
		}
		
		//
		// setsockopt + SO_RCVTIMEO :
		// 覆蓋掉 accept_socket 做 recv() 的 time out 時間
		//
		struct timeval timeout;
		timeout.tv_sec = 6;
		timeout.tv_usec = 0;
		if(setsockopt(accept_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1)
		{
			perror("setsockopt()");
			exit(1);
		}

		printf("recv() ... start\n");
		iDataNum = recv(accept_socket, buffer, sizeof(buffer), 0);
		if(iDataNum < 0)
		{
			perror("recv()");
			close(accept_socket);
			continue;
		}
		else
		{
			printf("recv() ... done\n");
		}
		printf("IP is %s\n", inet_ntoa(client_addr.sin_addr));
		printf("Port is %d\n", htons(client_addr.sin_port));
		printf("Recv length is %d\n", iDataNum);
		printf("Recv Data   is %s\n\n", buffer);
		send(accept_socket, buffer, sizeof(buffer), 0);
		
		printf("close client connect ... start\n");
		close(accept_socket);
		printf("close client connect ... done\n");
		
		if(strcmp(buffer, "quit") == 0)
		{
			break;
		}
	}

	//
	// Close Socket
	//
	printf("close server socket ... start\n");
	close(server_socket);
	printf("close server socket ... done\n");
	
	return 0;
}
