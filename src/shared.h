#ifndef SHARED_H
#define SHARED_H

#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 8889

struct cmsg_message
{
    int    command_type;
    char   nick[16];
    char   body[4092];
};

enum cmsg_message_type
{
    CMESG_EHLO=0,
    CMESG_OK=1,
    CMESG_LOGIN=2,
    CMESG_SND_MSG=3,
    CMESG_ERROR=4,
    CMESG_PRINT_LIST=5
};

int send_ehlo(int sockfd, struct cmsg_message* message);

void send_ok(int sockfd, struct cmsg_message* message);

void send_error(int sockfd, struct cmsg_message* message, char* error_msg);

void clear_buffer(struct cmsg_message* message);

void send_msg(int sockfd, char* data);

void send_print_list(int sockfd);

#endif /* SHARED_H */