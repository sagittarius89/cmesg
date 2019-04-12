#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/ipc.h> 
#include<sys/shm.h> 
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

#include "../shared.h"
#include "cmsg_list.h"
 
void connection_handler(void *);
void* client_list_ptr;
int shmid;
 
int main(int argc , char *argv[])
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    int key = getpid();

    shmid = shmget((key_t)key, sizeof(NODE)*MAX_CLIENTS, 0666 | IPC_CREAT);
    
    printf("Alloc shared memory...%d\n",shmid);
    client_list_ptr=cmsg_list_init(shmid);
    printf("Alloc shared memory success\n");

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
    }
    printf("Socket created\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( DEFAULT_PORT );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    printf("bind done\n");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
	
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
    {
        printf("Connection accepted\n");
        pthread_t thread_id;

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        printf("Handler assigned\n");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed\n"); 
        return 1;
    }
     
    dispose_server();
    
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    struct cmsg_message * client_message = malloc(sizeof(struct cmsg_message));
    client_list_ptr = shmat(shmid,0,0);

    //Receive a message from client
    while( (read_size = recv(sock , client_message , sizeof(struct cmsg_message), 0)) > 0 )
    {	
        if(read_size == sizeof(struct cmsg_message))
        {	
            process_request(sock, client_message);
            clear_buffer(client_message);
        }
    }
     
    if(read_size == 0)
    {
        printf("Client disconnected\n");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    } 

    //dispose_connection(sock);

    //free(client_message);

    return;
}

int login_user(int sockfd, struct cmsg_message* message)
{
    NODE client_data;

    if(cmsg_list_lookup_by_login(client_list_ptr,message->body)==1)
    {
        printf("nickname not available\n");
        send_error(sockfd,message,"nickname not available\n");
    }
    else
    {
        printf("user logged as %s\n", message->body);
        strcpy(client_data.nick,message->body);
        client_data.sockfd=sockfd;

        printf("shmid=%d\n", shmid);
        cmsg_list_add(client_list_ptr,client_data);

        send_ok(sockfd,message);
    }
}

void broadcast_msg(int sockfd, struct cmsg_message* message)
{
    char fullmsg[4098];
    char* nick = cmsg_list_lookup_by_sockfd(client_list_ptr, sockfd)->nick;

    memset(&fullmsg,0,sizeof(fullmsg));

    strcat(fullmsg,"@");
    strcat(fullmsg,nick);
    strcat(fullmsg,": ");
    strcat(fullmsg,message->body);

    cmsg_list_execute_for_all(client_list_ptr,send_msg,fullmsg);
}

void process_request(int sockfd, struct cmsg_message * client_message)
{
    switch(client_message->command_type)
    {
        case EHLO:
            send_ehlo(sockfd, client_message);
        break;

        case LOGIN:
            login_user(sockfd, client_message);
        break;

        case SND_MSG:
            broadcast_msg(sockfd, client_message);
        break;

        case PRINT_LIST:
            cmsg_list_print_list(client_list_ptr);
        break;
    }
}

void dispose_connection(sockfd)
{ 
    cmsg_list_remove_node(client_list_ptr,sockfd);
    close(sockfd);
}

void* close_connection(NODE* connection)
{
    close(connection->sockfd);
}

void dispose_server()
{
    cmsg_list_free_list(client_list_ptr,close_connection);
    shmctl(shmid,IPC_RMID,NULL);
}