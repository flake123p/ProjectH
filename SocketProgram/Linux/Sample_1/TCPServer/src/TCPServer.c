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
			exit(1);
		}
		else
		{
			printf("accept() ... done\n");
		}

		printf("recv() ... start\n");
		iDataNum = recv(accept_socket, buffer, sizeof(buffer), 0);
		if(iDataNum < 0)
		{
			perror("recv()");
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
