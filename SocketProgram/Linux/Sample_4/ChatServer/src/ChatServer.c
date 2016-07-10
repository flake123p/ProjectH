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
	int i = 0;
	
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
	#if 0
	struct timeval timeout;
	timeout.tv_sec  = 30;
	timeout.tv_usec = 0;
    if(setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1)
	{
        perror("setsockopt()");
        exit(1);
    }
	#endif
	
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
	socklen_t client_addr_len = sizeof(client_addr);
	char buffer[1024];
	int iDataNum ;

	//
	// Select
	//
	//int maxsock = 0;
	#define QUEUE_LENGTH 4
	int client_socket[QUEUE_LENGTH] = { 0 };
	int timeout_ctr = 0;
	while(1)
	{
//-------------------------------------------------------------------------------
		//
		// Scan and add all active socket fd for select()
		//
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(server_socket, &read_fds);
		int maxsock = server_socket;
        for (i = 0; i < QUEUE_LENGTH; i++) 
		{
            if (client_socket[i] != 0) 
			{
				FD_SET(client_socket[i], &read_fds);
				// Update max socket number
				if (client_socket[i] > maxsock) 
				{
					maxsock = client_socket[i];
				}

            }
        }

		//
		// Select, let it polling all socket fds 
		//
		int ret;
		struct timeval tv;
		tv.tv_sec	= 3;
		tv.tv_usec = 0;
		printf("select() ... start\n");
		ret = select(maxsock + 1, &read_fds, NULL, NULL, &tv);
		if (ret < 0)
		{
			perror("select()");
			exit(1);
		}
		else if (ret == 0)
		{
			printf("select() waitting timeout ... (R)(%d) maxsock = %d\n", timeout_ctr, maxsock);
			++timeout_ctr;
			continue;
		}
		printf("select() ... end\n");

		//
		// Select success(1/2), check server for new connection
		//
        if (FD_ISSET(server_socket, &read_fds)) 
		{
            accept_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
			if(accept_socket < 0)
			{
				perror("accept()");
				//exit(1);
				break;
			}

			printf("accept() ... done\n");
			printf("Server socket fd = %d\n", server_socket);
			printf("Accept socket fd = %d\n", accept_socket);

			for (i = 0; i < QUEUE_LENGTH; i++) 
			{
				if (client_socket[i] == 0) 
				{
					client_socket[i] = accept_socket;
					printf("new client[%d] %s:%d\n", i,
							inet_ntoa(client_addr.sin_addr),
							ntohs(client_addr.sin_port));
					break;
				}
			}
			if (i == QUEUE_LENGTH)
			{
				send(accept_socket, "sorry overload!", sizeof("sorry overload!"), 0);
				close(accept_socket);
				break;
			}
        }

		//
		// Select success(2/2), check every connection and receive data
		//
        for (i = 0; i < QUEUE_LENGTH; i++) 
		{
            if (client_socket[i] != 0 && FD_ISSET(client_socket[i], &read_fds)) 
			{
                ret = recv(client_socket[i], buffer, sizeof(buffer), 0);
                if (ret <= 0) 
				{        
                    printf("client[%d] close\n", i);
                    close(client_socket[i]);
                    FD_CLR(client_socket[i], &read_fds);
                    client_socket[i] = 0;
                } 
				else
				{     
                    printf("client[%d] msg:%s\n", i, buffer);

					//
					// Find all writalbe sockets, and send message to everyone.
					//
					fd_set write_fds = read_fds;
					ret = select(maxsock + 1, NULL, &write_fds, NULL, &tv);
					if (ret < 0)
					{
						perror("select()");
						exit(1);
					}
					else if (ret == 0)
					{
						printf("select() waitting timeout ... (W)(%d)\n", timeout_ctr);
						++timeout_ctr;
						continue;
					}
					int j = 0;
					for(j = 0; j < QUEUE_LENGTH; j++) 
					{ 
						if (client_socket[i] != 0 && FD_ISSET(client_socket[i], &write_fds)) 
						{
							if (send(client_socket[j], buffer, sizeof(buffer), 0) == -1) { 
								perror("send()"); 
							} 
						}
					} 
                }
            }
        }
//===============================================================================		

	}

	//
	// Close Socket
	//
	printf("close server socket ... start\n");
	close(server_socket);
	printf("close server socket ... done\n");
	
	return 0;
}

