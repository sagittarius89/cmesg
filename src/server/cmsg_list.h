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

void cmsg_list_print_list(void* head, char* buff);

NODE* cmsg_list_add(void* head, NODE data);

void cmsg_list_remove_node(void* head, int sockfd);

int cmsg_list_lookup_by_login(void* head, char* login);

NODE* cmsg_list_lookup_by_sockfd(void* head, int sockfd);

void cmsg_list_free_list(void* head, void*(close_func)(int));

void cmsg_list_execute_for_all(void* head, void*(func)(int, char*), char* data);

#endif /* CMSG_LIST_H */