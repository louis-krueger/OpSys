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

#define MAX_USERS 5
#define MAX_USERNAME_LEN 16
#define BUF_SIZE 1000

int isNewClient(struct sockaddr_in, struct sockaddr_in [], int);
void removeClient(struct sockaddr_in, struct sockaddr_in [], int [], int);

char users[MAX_USERS][MAX_USERNAME_LEN];

int main(int argc, char *argv[])
{
	/* DATA INITILIZATION */
	int sockfd, myport, nread, temp_addr_len, their_addr_len[MAX_USERS], i, nusers = 0, current;
  	struct sockaddr_in my_addr, temp_addr, their_addr[MAX_USERS];
  	char buf[BUF_SIZE], denyS[] = "Sorry the chat server is full.\r\n", 
		welcomeS[] = " has entered the chat!\r\n\0";
	/* END OF DATA INITILIZATION */
	/* ERROR CHECKING */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s portnum\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* END OF ERROR CHECKING */
	/* PORT SET UP */
	myport = atoi (argv[1]);
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
	temp_addr_len = sizeof (struct sockaddr_in);
	while (1)	
	{
		nread = recvfrom(sockfd, buf, BUF_SIZE, 0, 
			(struct sockaddr *) &temp_addr, &temp_addr_len);
		if (nread == 0)
			continue;
		if (isNewClient(temp_addr, their_addr, nusers))
		{
			if (nusers < 5)
			{
				their_addr[nusers] = temp_addr;
				their_addr_len[nusers] = temp_addr_len;
				strcpy(users[nusers], buf);
				strcat(users[nusers], ": ");
				nusers++;
				strcat(buf, welcomeS);
				for (i = 0; i < nusers; i++)
				{
					sendto(sockfd, buf, nread + sizeof(welcomeS), 0,
						(struct sockaddr *) &their_addr[i], their_addr_len[i]);
				}
				bzero(buf, BUF_SIZE);
				continue;
			}
			else
			{
				sendto(sockfd, denyS, sizeof(denyS), 0, (struct sockaddr *) &temp_addr, temp_addr_len);
				bzero(buf, BUF_SIZE);
				continue;
			}
		}
		for (i = 0; i < nusers; i++)
		{
			if (0 == memcmp(&their_addr[i], &temp_addr, sizeof(struct sockaddr_in)))
				current = i;
		}
		if (0 == strncmp(buf, "exit", 4))
		{
			close(sockfd);
			printf("Server listening end.\r\n");
			exit(EXIT_SUCCESS);
		}
		if (0 == strncmp(buf, "close", 5))
		{
			for (i = 0; i < nusers; i++)
			{
                        	sendto(sockfd, users[current], sizeof(users[current]), 0,
                                	(struct sockaddr *) &their_addr[i], their_addr_len[i]);
                        	sendto(sockfd, "has left the chat!\r\n", 20, 0,
                                	(struct sockaddr *) &their_addr[i], their_addr_len[i]);
			}
			removeClient(temp_addr, their_addr, their_addr_len, nusers);
			nusers--;
			continue;
		}
		buf[nread] = '\0';
		for (i = 0; i < nusers; i++)
		{
			if (0 != memcmp(&their_addr[i], &temp_addr, sizeof(struct sockaddr_in)))
			{
				sendto(sockfd, users[current], sizeof(users[current]), 0, (struct sockaddr *) &their_addr[i], their_addr_len[i]);
				sendto(sockfd, buf, nread, 0, (struct sockaddr *) &their_addr[i], their_addr_len[i]);
			}
		}
		bzero(buf, BUF_SIZE);
	}
}

int isNewClient(struct sockaddr_in new_addr, struct sockaddr_in old_addr[], int nusers)
{
	int i;
	for (i = 0; i < nusers; i++)
	{
		if (0 == memcmp(&old_addr[i], &new_addr, sizeof(struct sockaddr_in)))
			return 0;
	}
	return 1;
}

void removeClient(struct sockaddr_in new_addr, struct sockaddr_in old_addr[], int old_addr_len[], int nusers)
{
	int i;
	for (i = 0; i < nusers; i++)
	{
		if ((i == nusers - 1) && 0 == memcmp(&old_addr[i], &new_addr, sizeof(struct sockaddr_in)))
		{
			break;
		}
		else if (0 == memcmp(&old_addr[i], &new_addr, sizeof(struct sockaddr_in)))
		{
			old_addr[i] = old_addr[nusers - 1];
			old_addr_len[i] = old_addr_len[nusers - 1];
			strcpy(users[i], users[nusers - 1]);
			break;
		}
	}
	return;
}
