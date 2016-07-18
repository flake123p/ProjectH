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

/*
 * [function description]
 * htons() means: host to Network Short
 * other: htons(), htonl(), ntohs(), ntohl()
 */

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
	// Send / Receive Message
	//
	char sendbuf[1024];
	char recvbuf[1024];
	printf("Input client message:>");
	scanf("%s", sendbuf);

	send(client_socket, sendbuf, sizeof(sendbuf), 0);
	recv(client_socket, recvbuf, sizeof(recvbuf),0);
	printf("recv data from server is: %s\n", recvbuf);

	//
	// Close Socket
	//
	close(client_socket);

	return 0;
}
