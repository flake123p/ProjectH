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
	int peer_socket;
	if((peer_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
		exit(1);
	}
	
	//
	// Bind socket with IP and Port
	//
	struct sockaddr_in self_addr;
	memset((void*)&self_addr, 0, sizeof(self_addr));
	self_addr.sin_family = AF_INET;
	self_addr.sin_port   = htons(SERVER_PORT);
	self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(peer_socket, (struct sockaddr *)&self_addr, sizeof(self_addr)) < 0)
	{
		perror("connect()");
		exit(1);
	}
	
	//
	// Send / Receive Message
	//
	struct sockaddr_in input_peer_addr;
	int peer_len = sizeof(input_peer_addr);
	char recvbuf[1024];
	char sendbuf[1024];
	while(1)
	{			
		printf("Input your message:>");
		scanf("%s",sendbuf);
		sendto(peer_socket, sendbuf, 1024, 0, (struct sockaddr *)&self_addr, sizeof(self_addr));
		recvfrom(peer_socket, recvbuf, 1024, 0, (struct sockaddr *)&input_peer_addr, (socklen_t*)&peer_len);
		printf("Receive message: %s\n\n",recvbuf);
	}
	
	return 0;
}