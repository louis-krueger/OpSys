/* Written by Samuel Scheel & Louis Krueger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define EXIT_LEN 4
#define CLOSE_LEN 5
#define MAX_USERS 5
#define MAX_USERNAME_LEN 16
#define BUF_SIZE 1000

int main(int argc, char *argv[])
{
        /* DATA INITILIZATION */
        int sockfd, serv_port, nread, pid, c, nout;
        struct sockaddr_in serv_addr;
	struct hostent *server;
        char buf_in[BUF_SIZE], name[MAX_USERNAME_LEN], buf_out[BUF_SIZE];
        /* END OF DATA INITILIZATION */
        /* ERROR CHECKING */
        if (argc < 4)
        {
                fprintf(stderr, "Usage: %s server portnum username\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        /* END OF ERROR CHECKING */
        /* PORT SET UP */
        serv_port = atoi(argv[2]);
        sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr, "Failed to find server!\r\n");
		exit(EXIT_FAILURE);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
   	serv_addr.sin_family = AF_INET;
   	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   	serv_addr.sin_port = htons(serv_port);
   	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
	{
		close(sockfd);
      		fprintf(stderr, "Failed to connect to server!\r\n");
      		exit(EXIT_FAILURE);
   	}
        /* END OF PORT SET UP */
	if ((pid = fork()) < 0)
	{
		close(sockfd);
		printf("Failed to fork!\r\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		strcpy(name, argv[3]);
		write(sockfd, name, sizeof(name));
		nout = 0;
		while (((c = getchar()) != EOF))
		{
			buf_out[nout] = c;
			nout++;
			if (c == '\n')
			{
				buf_out[nout] = '\0';
				nout++;
				write(sockfd, buf_out, nout);
				nout = 0;
			}
		}
	}
	else
	{
		while(1)
		{
			nread = read(sockfd, buf_in, BUF_SIZE);
			if (nread == 0)
                        	continue;
			if (0 == strncmp(buf_in, "exit", 4))
			{
				printf("The server has closed.\r\n");
				close(sockfd);
				exit(EXIT_SUCCESS);
			}
			if (0 == strncmp(buf_in, "close", 5))
                        {
                                printf("You have left the server.\r\n");
                                close(sockfd);
                                exit(EXIT_SUCCESS);
                        }
			buf_in[nread] = '\0';
			printf("%s", buf_in);
		}
	}
}
