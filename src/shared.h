#ifndef SHARED_H
#define SHARED_H

#define DEFAULT_PORT 8889

struct cmsg_message
{
    int    command_type;
    char   nick[16];
    char   body[4092];
};

enum cmsg_message_type
{
    EHLO=0,
    OK=1,
    LOGIN=2,
    SND_MSG=3,
    ERROR=4,
    PRINT_LIST=5
};

int send_ehlo(int sockfd, struct cmsg_message* message);

void send_ok(int sockfd, struct cmsg_message* message);

void send_error(int sockfd, struct cmsg_message* message, char* error_msg);

void clear_buffer(struct cmsg_message* message);

void send_msg(int sockfd, char* data);

void send_print_list(int sockfd);

#endif /* SHARED_H */