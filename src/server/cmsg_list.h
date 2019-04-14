#ifndef CMSG_LIST_H
#define CMSG_LIST_H

#define MAX_CLIENTS 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

typedef struct {
    int sockfd;
    char nick[16];
} NODE;

void* cmsg_list_init(int shmid);

void cmsg_list_print_list(NODE* head, char* buff);

NODE* cmsg_list_add(NODE* head, NODE data);

void cmsg_list_remove_node(NODE* head, int sockfd);

int cmsg_list_lookup_by_login(NODE* head, char* login);

NODE* cmsg_list_lookup_by_sockfd(NODE* head, int sockfd);

void cmsg_list_free_list(NODE* head, void*(close_func)(int));

void cmsg_list_execute_for_all(NODE* head, void*(func)(int, char*), char* data);

#endif /* CMSG_LIST_H */