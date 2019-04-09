#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include "../shared.h"

int receive_ehlo(int sockfd, struct cmsg_message* message);
int login_user(int sockfd, struct cmsg_message* message, char* login);
void server_msg_handler(int sockfd);

int main(int argc, char *argv[])
{
	int sockfd = 0, n = 0;
	struct cmsg_message* message = malloc(sizeof(struct cmsg_message));
	struct sockaddr_in serv_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(DEFAULT_PORT);

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	}

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	}

	clear_buffer(message);
	
	send_ehlo(sockfd, message);
	
	clear_buffer(message);
	
	receive_ehlo(sockfd, message);

	clear_buffer(message);

	login_user(sockfd, message, argv[2]); 

	int pid = fork();

	if(pid==0)
	{
		server_msg_handler(sockfd);
		return 0;
	}

	parse_commands(sockfd);

	close(sockfd);

	return 0;
}

int receive_ehlo(int sockfd, struct cmsg_message* message)
{
    int n=read(sockfd, message, sizeof(message));

	if(n < 0)
	{
		printf("\n EHLO error \n");
		return -1;
	}

	if(message->command_type==EHLO)
	{
		printf("\n EHLO ok \n");

		return 1;
	}
	else
	{
		printf("\n EHLO failed \n");
	}

	return -1;
}

int login_user(int sockfd, struct cmsg_message* message, char* login)
{
		message->command_type = LOGIN;
		strcpy(message->body , login);

		write(sockfd, message, sizeof(struct cmsg_message));

		int n=read(sockfd, message, sizeof(message));

		if(n < 0)
		{
			printf("\n LOGIN error, connection failed \n");
			return -1;
		}

		if(message->command_type==OK)
		{
			printf("\n You are logged in with login %s \n", login);

			return 1;
		}
		else if(message->command_type==ERROR)
		{
			printf("\n LOGIN failed, %s \n", message->body);
		}

	return -1;
}

void server_msg_handler(int sockfd)
{
	struct cmsg_message message;
	clear_buffer(&message);

	while(1)
	{
		int n=read(sockfd, &message, sizeof(message));

		if(n < 0)
		{
			printf("\n SND_MSG error \n");
			return;
		}

		if(message.command_type==SND_MSG)
		{
			printf("%s\n", message.body);

			fflush(stdout);
		}
		else
		{
			/*process other here*/
		}
	}
}

void parse_commands(int sockfd)
{
	char cmd[4092];

	while(1)
	{
		memset(&cmd,0,sizeof(cmd));
		fgets(cmd,4092,stdin);

		if(cmd[0]=='!')
		{
			send_print_list(sockfd);
		}
		else
		{
			send_msg(sockfd,cmd);
		}
	}
}