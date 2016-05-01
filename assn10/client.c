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

//char* hostname(char*);

int main(int argc, char *argv[])
{
        /* DATA INITILIZATION */
        int sockfd, myport, nread, their_addr_len, pid, c, nout;
        struct sockaddr_in my_addr, their_addr;
        char buf_in[BUF_SIZE], name[MAX_USERNAME_LEN], buf_out[BUF_SIZE];
        /* END OF DATA INITILIZATION */
        /* ERROR CHECKING */
        if (argc < 4)
        {
                fprintf(stderr, "Usage: %s server portnum username\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        /* END OF ERROR CHECKING */
	strcpy(name, argv[3]);
        /* PORT SET UP */
	//printf("Resolved host name: %s\r\n", hostname(argv[1]));
        myport = atoi (argv[2]);
        sockfd = socket (AF_INET, SOCK_DGRAM, 0);
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons (myport);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset (&(my_addr.sin_zero), '\0', 8);
        /* END OF PORT SET UP */
        if (bind (sockfd, (struct sockaddr *) &my_addr, sizeof (struct sockaddr)))
        {
                close (sockfd);
                fprintf (stderr, "Failed to bind socket!\n");
                exit(EXIT_FAILURE);
        }
        else
        {
                printf ("Server listening on port %d\n", myport);
        }
	their_addr_len = sizeof (struct sockaddr_in);
	if (getnameinfo((struct sockaddr *) &their_addr, their_addr_len, argv[1], sizeof(argv[1]), 0, 0, 0 ))
	{
		close(sockfd);
		fprintf (stderr, "Failed to something!\n");
                exit(EXIT_FAILURE);
	}	
	if ((pid = fork()) < 0)
	{
		printf("Failed to fork!\r\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		printf("IO PROCESS\r\n");
		nout = 0;
		while (((c = getchar()) != EOF))
		{
			buf_out[nout] = c;
			nout++;
			if (c == '\n')
			{
				buf_out[nout] = '\0';
				nout++;
				printf("sending\r\n");
				sendto(sockfd, buf_out, nout, 0, (struct sockaddr *) &their_addr, their_addr_len);
				nout = 0;
			}
		}
	}
	else
	{
		printf("READING PROCESS\r\n");
		while(1)
		{
			nread = recvfrom(sockfd, buf_in, BUF_SIZE, 0,
                        	(struct sockaddr *) &their_addr, &their_addr_len);
			if (nread == 0)
                        	continue;
			buf_in[nread] = '\0';
			printf("%s", buf_in);
		}
	}
}
/*
char* hostname(char *hostname)
{
	char *ip;
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	if ((he = gethostbyname(hostname)) == NULL)
		return NULL;
	addr_list = (struct in_addr **) he->h_addr_list;
	for (i = 0; addr_list[i] != NULL; i++)
	{
		strcpy(ip, inet_ntoa(*addr_list[i]));
	}
	return ip;
} */
