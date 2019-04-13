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
#include <signal.h>
#include <pthread.h>

#include "../shared.h"
#include "utils.h"
#include "display.h"
#include "input.h"


int receive_ehlo(int sockfd, struct cmsg_message* message);
int login_user(int sockfd, struct cmsg_message* message);
void server_msg_handler(int sockfd);
void parse_commands(int sockfd);
void join_to_chat(int sockfd, struct cmsg_message* message);
void sigint_handler(int sig_num);

struct options* options;

int main(int argc, char *argv[])
{
    int n = 0;
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

    if((options->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

    if(connect(options->sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    //init display
    initialize_curses();
    signal(SIGINT, sigint_handler);
    signal(SIGWINCH, resize_handler);

    pthread_t thread_id;

    if( pthread_create( &thread_id , NULL ,  server_msg_handler , options->sockfd))
    {
        w_print_err("could not create thread");
        return 1;
    }

	join_to_chat(options->sockfd,message);

    parse_commands(options->sockfd);

    if (options->sockfd) {
        close(options->sockfd);
    }

    wrefresh(chatWin);
    endwin();

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
        w_print_err("EHLO error");
        return -1;
    }

    if(message->command_type==EHLO)
    {
        w_print_info("EHLO ok");

        return 1;
    }
    else
    {
        w_print_err("EHLO failed");
    }

    return -1;
}

int login_user(int sockfd, struct cmsg_message* message)
{
    message->command_type = LOGIN;
    strcpy(message->body , (char*)options->nick);

    write(sockfd, message, sizeof(struct cmsg_message));

    int n=read(sockfd, message, sizeof(message)); 

    char tmp[100];

    if(n < 0)
    {
        w_print_err("LOGIN error, connection failed");
        return -1;
    }

    if(message->command_type==OK)
    {
        strcat(tmp, "Welcome! You are logged in with login ");
        strcat(tmp, options->nick);

        w_print_info(tmp);

        return 1;
    }
    else if(message->command_type==ERROR)
    {
        strcat(tmp, "LOGIN failed, ");
        strcat(tmp, message->body);

        w_print_err(tmp);
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
            w_print_err("SND_MSG error");
            return;
        }

        if(message.command_type==SND_MSG)
        {
            w_print_info(message.body);
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
        memset(cmd,0,sizeof(cmd));
        user_input(cmd);

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

/* (CTRL+C) */
void sigint_handler(int sig_num) {
   if (options->sockfd) {
      close(options->sockfd);
   }
   wrefresh(chatWin);
   endwin();
   exit(0);
}