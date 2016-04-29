#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#define BUFF_SIZE 1000

int main(int argc, char *argv[])
{
	int myport, sockfd;
	struct sockaddr_in my_addr, their_addr;
	
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s portnum\n", argv[0]);
		exit(EXIT_FAILURE);
	}

}



//port numbers 32500>
