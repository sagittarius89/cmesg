#include "shared.h"

int send_ehlo(int sockfd, struct cmsg_message* message)
{
    message->command_type = (int)CMESG_EHLO;
    strcpy(message->body, "EHLO");

    write(sockfd, message, sizeof(struct cmsg_message));
}

void send_ok(int sockfd, struct cmsg_message* message)
{
    message->command_type = (int)CMESG_OK;
    write(sockfd, message, sizeof(struct cmsg_message));
}

void send_error(int sockfd, struct cmsg_message* message, char* error_msg)
{
    message->command_type = (int)CMESG_ERROR;
    strcpy(message->body, error_msg);

    write(sockfd, message, sizeof(struct cmsg_message));
}

void clear_buffer(struct cmsg_message* message)
{
    memset(message, 0, sizeof(struct cmsg_message));
}

void send_msg(int sockfd, char* data)
{
    struct cmsg_message message;
    clear_buffer(&message);

    message.command_type = (int)CMESG_SND_MSG;
    strcpy(message.body, data);

    write(sockfd, &message, sizeof(struct cmsg_message));
}

void send_print_list(int sockfd)
{
    struct cmsg_message message;
    clear_buffer(&message);

    message.command_type = (int)CMESG_PRINT_LIST;

    write(sockfd, &message, sizeof(struct cmsg_message));
}