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
	int client_socket;
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(1); 		
	}

	//
	// Connect to server by IP and Port
	//
	struct sockaddr_in server_addr;
	memset((void*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("connect()");
		exit(1);
	}
	else
	{
		printf("Connect success ...\n");
	}
	
	//
	//Receive Message
	//
	int ret;
	char recvbuf[1024];
	while(1)
	{
		ret = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
		if (ret <= 0) 
		{		 
			printf("server closed\n");
			break;
		} 

		printf("recv data from server is: %s\n", recvbuf);

		if (strcmp("quit", recvbuf) == 0)
			break;
	}
	
	//
	// Close Socket
	//
	close(client_socket);

	return 0;
}
