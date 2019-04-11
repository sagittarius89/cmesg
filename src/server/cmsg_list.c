#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h> 

#include "cmsg_list.h"

void* cmsg_list_init(int shmid) {
    void*  addr_id = shmat(shmid,0,0);
    memset(addr_id, 0, sizeof(NODE)*MAX_CLIENTS);

    return addr_id;
}

void cmsg_list_print_list(void* head) {
    for(int i=0; i<sizeof(NODE)*MAX_CLIENTS; i+=sizeof(NODE))
    {
        NODE* temp = head+i;

        if(temp->sockfd != 0)
            printf("%d: %s\n", temp->sockfd, temp->nick);
    }
}

NODE* cmsg_list_add(void* head, NODE data) {
    NODE* temp=0;

    cmsg_list_print_list(head);

    for(int i=0; i<sizeof(NODE)*MAX_CLIENTS; i+=sizeof(NODE))
    {
        temp=head+i;

        if(temp->sockfd == 0)
        {
            break;
        }
    }

    if(temp->sockfd != 0)
    {
        return -1;
    }

    temp->sockfd = data.sockfd;
    strcpy(temp->nick,data.nick);

    printf("Node added\n");

    return temp;
}

void cmsg_list_remove_node(void* head, int sockfd) {
    if(sockfd<1)
        return;

    NODE* temp=0;

    for(int i=0; i<sizeof(NODE)*MAX_CLIENTS; i+=sizeof(NODE))
    {
        NODE* temp=head+i;

        if(temp->sockfd==sockfd)
            break;
    }

    if(temp->sockfd != 0)
        memset(temp, '0',sizeof(NODE));
}

int cmsg_list_lookup_by_login(void* head, char* login) {
    NODE* temp;

    for(int i=0; i<sizeof(NODE)*MAX_CLIENTS; i+=sizeof(NODE))
    {
        NODE* temp=head+i;

        if(strcmp(login,temp->nick)==0)
            return 1;
    }

    return 0;
}

NODE* cmsg_list_lookup_by_sockfd(void* head, int sockfd) {
    NODE* temp;

    for(int i=0; i<sizeof(NODE)*MAX_CLIENTS; i+=sizeof(NODE))
    {
        NODE* temp=head+i;

        if(temp->sockfd==sockfd)
            return temp;
    }

    return -1;
}

void cmsg_list_free_list(void* head,void* (close_func)(int))
{
    NODE* temp;

    for(int i=0; i<sizeof(NODE)*MAX_CLIENTS; i+=sizeof(NODE))
    {
        NODE* temp=head+i;

        if(temp->sockfd>0)
           close_func(temp->sockfd);
    }
}

void cmsg_list_execute_for_all(void* head,void* (func)(int,char*), char* data)
{
    NODE* temp;

    for(int i=0; i<sizeof(NODE)*MAX_CLIENTS; i+=sizeof(NODE))
    {
        NODE* temp=head+i;

        if(temp->sockfd>0)
           func(temp->sockfd, data);
    }
}