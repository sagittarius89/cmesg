#include "cmsg_list.h"

void* cmsg_list_init(int shmid)
{
    void* addr_id = shmat(shmid, 0, 0);
    memset(addr_id, 0, sizeof(NODE) * MAX_CLIENTS);

    return addr_id;
}

void cmsg_list_print_list(NODE* head, char* buff)
{
    NODE* temp;

    for (int i = 0; i < MAX_CLIENTS; i ++) {
        temp = &head[i];

        if (temp->sockfd != 0) {
            printf("%d: %s\n", temp->sockfd, temp->nick);

            strcat(buff, "@");
            strcat(buff, temp->nick);
            strcat(buff, " ");
        }
    }
}

NODE* cmsg_list_add(NODE* head, NODE data)
{
    NODE* temp;

    for (int i = 0; i < MAX_CLIENTS; i ++) {
        temp = &head[i];

        if (temp->sockfd == 0) {
            break;
        }
    }

    if (temp->sockfd != 0) {
        return NULL;
    }

    temp->sockfd = data.sockfd;
    strcpy(temp->nick, data.nick);

    printf("Node added\n");

    return temp;
}

void cmsg_list_remove_node(NODE* head, int sockfd)
{
    if (sockfd < 1)
        return;

    NODE* temp;

    for (int i = 0; i < MAX_CLIENTS; i ++) {
        temp = &head[i];

        if (temp->sockfd == sockfd)
            break;
    }

    if (temp->sockfd != 0)
        memset(temp, 0, sizeof(NODE));
}

int cmsg_list_lookup_by_login(NODE* head, char* login)
{
    NODE* temp;

    for (int i = 0; i < MAX_CLIENTS; i ++) {
        temp = &head[i];

        if (strcmp(login, temp->nick) == 0)
            return 1;
    }

    return 0;
}

NODE* cmsg_list_lookup_by_sockfd(NODE* head, int sockfd)
{
    NODE* temp;

    for (int i = 0; i < MAX_CLIENTS; i ++) {
        temp = &head[i];

        if (temp->sockfd == sockfd)
            return temp;
    }

    return NULL;
}

void cmsg_list_free_list(NODE* head, void*(close_func)(int))
{
    NODE* temp;

    for (int i = 0; i < MAX_CLIENTS; i ++) {
        temp = &head[i];

        if (temp->sockfd > 0)
            close_func(temp->sockfd);
    }
}

void cmsg_list_execute_for_all(NODE* head, void*(func)(int, char*), char* data)
{
    NODE* temp;

    for (int i = 0; i < MAX_CLIENTS; i ++) {
        temp = &head[i];

        if (temp->sockfd > 0)
            func(temp->sockfd, data);
    }
}