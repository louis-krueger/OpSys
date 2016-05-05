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
#define COLOR_LEN 5

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m\n"

struct user
{
	struct sockaddr_in addr;
	int addr_len;
	char name[MAX_USERNAME_LEN];
	char color[COLOR_LEN];
};

int isNewClient(struct sockaddr_in, int);
void removeClient(struct sockaddr_in, int);

struct user user_tab[MAX_USERS];
char color_pool[7][COLOR_LEN] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};

int main(int argc, char *argv[])
{
	/* DATA INITILIZATION */
	int sockfd, myport, nread, temp_addr_len, i, nusers = 0, current = 0, color_index = 0;;
  	struct sockaddr_in my_addr, temp_addr;
  	char buf[BUF_SIZE], denyS[] = "Sorry the chat server is full.\r\n", 
		welcomeS[] = " has entered the chat!";
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
	// Infinite loop of reading and handling users. Ends when user sends "exit"
	{
		nread = recvfrom(sockfd, buf, BUF_SIZE, 0, 
			(struct sockaddr *) &temp_addr, &temp_addr_len);
		if (nread == 0)
			continue;
		if (isNewClient(temp_addr, nusers))
		// If we have a new client, check if we have space then add them to the chat.
		{
			if (nusers < 5)
			// If we have enough space, add new client into our "user_tab" to track their information
			{
				/* SET UP NEW INFORMATION IN USER_TAB */
				user_tab[nusers].addr = temp_addr;
				user_tab[nusers].addr_len = temp_addr_len;
				strcpy(user_tab[nusers].name, buf);
				strcat(user_tab[nusers].name, ": ");
				strcpy(user_tab[nusers].color, color_pool[(color_index++) % 6]);
				current = nusers;
				nusers++;
				/* END OF NEW INFORMATION SET UP */
				/* SEND NEW USER LIST OF COMMANDS AND CURRENT USERS */
				sendto(sockfd, "Commands:\nclose\nexit\nlist\n?\n", 28, 0,
                                        (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
				sendto(sockfd, "Current Users:\n", 15, 0,
                                                (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
                        	for (i = 0; i < nusers; i++)
                        	{
                                	sendto(sockfd, user_tab[i].color, sizeof(user_tab[i].color), 0,
                                                (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
                                	sendto(sockfd, user_tab[i].name, sizeof(user_tab[i].name), 0,
                                        	(struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
                                	sendto(sockfd, RESET, sizeof(RESET), 0,
                                        	(struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
                        	}
				/* END OF START UP HELP */
				/* INFORM OLD USERS OF NEW USERS ENTRANCE INTO CHAT */
				strcat(buf, welcomeS);
				for (i = 0; i < nusers; i++)
				{
					if (0 != memcmp(&user_tab[i].addr, &temp_addr, sizeof(struct sockaddr_in)))
					{
						sendto(sockfd, user_tab[current].color, sizeof(user_tab[current].color), 0,
                                                	(struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
						sendto(sockfd, buf, nread + sizeof(welcomeS), 0,
							(struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
						sendto(sockfd, RESET, sizeof(RESET), 0,
                                                	(struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
					}
					else
					{
						sendto(sockfd, "Welcome to the chat!\n", 21, 0,
                                                        (struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
					}
				}
				/* END OF OLD USER INFORMING */
				bzero(buf, BUF_SIZE);
				continue;
			}
			else
			// If we do not have enough space, deny them.
			{
				sendto(sockfd, denyS, sizeof(denyS), 0, (struct sockaddr *) &temp_addr, temp_addr_len);
				sendto(sockfd, "close", 5, 0, (struct sockaddr *) &temp_addr, temp_addr_len);
				bzero(buf, BUF_SIZE);
				continue;
			}
		}
		for (i = 0; i < nusers; i++)
		// Find the array index of the new user for future use
		{
			if (0 == memcmp(&user_tab[i].addr, &temp_addr, sizeof(struct sockaddr_in)))
				current = i;
		}
		if (0 == strncmp(buf, "exit", 4))
		// If a user closes the entire server
		{
			for (i = 0; i < nusers; i++)
			// Tells every client that the server has closed.
			// If they have a protocol to deal with it, they will close as well.
                	{
                                sendto(sockfd, buf, nread, 0, (struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
                	}
			close(sockfd);
			printf("Server listening end.\r\n");
			exit(EXIT_SUCCESS);
		}
		if (0 == strncmp(buf, "close", 5))
		// If a user closes their own connection
		{
			for (i = 0; i < nusers; i++)
			// Let every other user know that user has left the chat.
			{
				if (0 != memcmp(&user_tab[i].addr, &temp_addr, sizeof(struct sockaddr_in)))
				{
                        		sendto(sockfd, user_tab[current].name, sizeof(user_tab[current].name), 0,
                                		(struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
                        		sendto(sockfd, "has left the chat!\r\n", 20, 0,
                                		(struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
				}
				else
				// Sends close command back to leaving user.
				// If they have a protocol to deal with it, they will close.
				{
					sendto(sockfd, buf, nread, 0, (struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
				}
			}
			/* UPDATE USER_TAB INFORMATION */
			removeClient(temp_addr, nusers);
			nusers--;
			/* END OF USER_TAB UPDATES */
			continue;
		}
		if (0 == strncmp(buf, "list", 4))
		// If a user wishes to see every user in the chat
                {
			sendto(sockfd, "Current Users:\n", 15, 0,
                                                (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
			for (i = 0; i < nusers; i++)
			// Lists every user in their appropriate color
                        {
				sendto(sockfd, user_tab[i].color, sizeof(user_tab[i].color), 0,
                                                (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
                                sendto(sockfd, user_tab[i].name, sizeof(user_tab[i].name), 0,
                                        (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
                                sendto(sockfd, RESET, sizeof(RESET), 0,
                                        (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
                	}
			continue;
		}
		if (0 == strncmp(buf, "?", 1))
		// If a user wishes to see the available commands */
		{
			sendto(sockfd, "Commands:\nclose\nexit\nlist\n?\n", 28, 0,
                                        (struct sockaddr *) &user_tab[current].addr, user_tab[current].addr_len);
			continue;
		}
		/* IF THE CODE REACHES HERE, IT IS A STANDARD MESSAGE */
		/* SEND MESSAGE TO EVERY USER */
		for (i = 0; i < nusers; i++)
		// Sends every user the message with the author's name and assigned color
		{
			if (0 != memcmp(&user_tab[i].addr, &temp_addr, sizeof(struct sockaddr_in)))
			{
				sendto(sockfd, user_tab[current].color, sizeof(user_tab[current].color), 0,
                                                (struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
				sendto(sockfd, user_tab[current].name, sizeof(user_tab[current].name), 0, 
						(struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
				sendto(sockfd, strcat(buf, RESET), nread + sizeof(RESET), 0, 
						(struct sockaddr *) &user_tab[i].addr, user_tab[i].addr_len);
			}
		}
		/* END OF SENDING MESSAGE TO EVERY USER */
		bzero(buf, BUF_SIZE);
	}
}

int isNewClient(struct sockaddr_in new_addr, int nusers)
// Simple boolean function to determine if the user is a new client.
{
	int i;
	for (i = 0; i < nusers; i++)
	{
		if (0 == memcmp(&user_tab[i].addr, &new_addr, sizeof(struct sockaddr_in)))
			return 0;
	}
	return 1;
}

void removeClient(struct sockaddr_in new_addr, int nusers)
// Reorders array to remove the paramater client
{
	int i;
	for (i = 0; i < nusers; i++)
	{
		if ((i == nusers - 1) && 0 == memcmp(&user_tab[i].addr, &new_addr, sizeof(struct sockaddr_in)))
		{
			break;
		}
		else if (0 == memcmp(&user_tab[i].addr, &new_addr, sizeof(struct sockaddr_in)))
		{
			user_tab[i].addr = user_tab[nusers - 1].addr;
			user_tab[i].addr_len = user_tab[nusers - 1].addr_len;
			strcpy(user_tab[i].name, user_tab[nusers - 1].name);
			break;
		}
	}
	return;
}
