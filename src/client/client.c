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
#include "utils.h"

int receive_ehlo(int sockfd, struct cmsg_message* message);
int login_user(int sockfd, struct cmsg_message* message);
void server_msg_handler(int sockfd);
void parse_commands(int sockfd);
void join_to_chat(int sockfd, struct cmsg_message* message);

struct options* options;

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    struct cmsg_message* message;
    struct sockaddr_in serv_addr;

	options=parse_args(argc,argv);

	if(options->show_help)
	{
		print_help();
		return 0;
	}

	if(!validate_args(options))
	{
		return 1;
	}

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
	message = malloc(sizeof(struct cmsg_message));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(options->port);

    if(inet_pton(AF_INET, (char*)options->address, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    int pid = fork();

    if(pid==0)
    {
        server_msg_handler(sockfd);
        return 0;
    }

	join_to_chat(sockfd,message);

    parse_commands(sockfd);

    close(sockfd);

	free(message);
	free(options);

    return 0;
}

void join_to_chat(int sockfd, struct cmsg_message* message)
{
    send_ehlo(sockfd, message);

    receive_ehlo(sockfd, message);

    login_user(sockfd, message);
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

int login_user(int sockfd, struct cmsg_message* message)
{
    message->command_type = LOGIN;
    strcpy(message->body , (char*)options->nick);

    write(sockfd, message, sizeof(struct cmsg_message));

    int n=read(sockfd, message, sizeof(message)); 

    if(n < 0)
    {
        printf("\n LOGIN error, connection failed \n");
        return -1;
    }

    if(message->command_type==OK)
    {
        printf("\n You are logged in with login %s \n", options->nick);

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